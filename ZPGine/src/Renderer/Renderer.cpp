#include "Renderer.h"

#include "Culling/Volume.h"
#include "RenderCommand.h"
#include "Debug/Asserter.h"
#include "Model/Model.h"
#include "Renderer/DrawBatch.h"
#include "Renderer/DrawCommand.h"
#include "Renderer/RenderPass.h"
#include "Renderer/RenderPass/BloomRenderPass.h"
#include "Renderer/RenderPass/DirectionalLightShadowRenderPass.h"
#include "Renderer/RenderPass/LightVolume/EnvironmentLightRenderPass.h"
#include "Renderer/RenderPass/LightVolume/PointLightRenderPass.h"
#include "Renderer/RenderPass/LightVolume/SpotLightRenderPass.h"
#include "Renderer/RenderPass/LightVolume/TransitionRenderPass.h"
#include "Renderer/RenderPass/PointLightShadowRenderPass.h"
#include "Renderer/RenderPass/SpotLightShadowRenderPass.h"
#include "Renderer/RenderPass/GeometryRenderPass.h"
#include "Renderer/RenderPass/LightingRenderPass.h"
#include "Entity/Entity.h"
#include "Profiling/Instrumentor.h"
#include "RenderPass/DeferredLightingBlinnPhongRenderPass.h"
#include "RenderPass/EntityRenderPass.h"
#include "RenderPass/ForwardBlinnPhongRenderPass.h"
#include "RenderPass/LightVolumeDeferredLightingBlinnPhongRenderPass.h"
#include "RenderPass/SkyRenderPass.h"
#include "Resource/CommonResources.h"
#include "Scene/Scene.h"
#include "Texture/Texture2D.h"


namespace ZPG
{

#define ADD_DYNAMIC_COMMANDS(where, commands) \
    AddCommands(s->m_RenderContext.Queues.where, s->m_RenderContext.Batches.where, commands)

#define ADD_STATIC_COMMANDS(where, commands) \
    AddCommands(s->m_RenderContext.StaticQueues.where, s->m_RenderContext.StaticBatches.where, commands)

#define ADD_COMMANDS(isStatic, where, commands) \
    if (isStatic) \
        ADD_STATIC_COMMANDS(where, drawCommands); \
    else \
        ADD_DYNAMIC_COMMANDS(where, drawCommands);


    Renderer::Renderer(const RenderContextSpecification& renderContextSpecification)
        : m_RenderContext(renderContextSpecification)
    {   
    }

    Renderer::~Renderer()
    {
    }

    void Renderer::Init()
    {
        ZPG_PROFILE_FUNCTION();
       
        RenderContextSpecification renderContextSpec{};
        renderContextSpec.BatchSize = 2 * 1024;
        renderContextSpec.PointLightCapacity = 256;
        renderContextSpec.SpotLightCapacity = 256;

        s = new Renderer(renderContextSpec);

        PushRenderPass(new DirectionalLightShadowRenderPass());
        // PushRenderPass(new SpotLightShadowRenderPass());
        // PushRenderPass(new PointLightShadowRenderPass());
        PushRenderPass(new GeometryRenderPass());
        PushRenderPass(new TransitionRenderPass());
        // PushRenderPass(new LightingRenderPass());
        // PushRenderPass(new ForwardBlinnPhongRenderPass());
        PushRenderPass(new DeferredLightingBlinnPhongRenderPass());
        // PushRenderPass(new LightVolumeDeferredLightingBlinnPhongRenderPass());
        // PushRenderPass(new EnvironmentLightRenderPass());
        // PushRenderPass(new PointLightRenderPass());
        // PushRenderPass(new SpotLightRenderPass());
        // PushRenderPass(new EntityRenderPass());
        PushRenderPass(new SkyRenderPass());
        PushRenderPass(new BloomRenderPass());


        // this doesn't fit anywhere else
        s->m_RenderContext.SSBO.ProcessingSSBO.SetGamma(2.2f);
        s->m_RenderContext.SSBO.ProcessingSSBO.SetExposure(1.0f);
    }

    void Renderer::Shutdown()
    {
        ZPG_PROFILE_FUNCTION();
        ZPG_CORE_ASSERT(s != nullptr, "Renderer wasn't initialized.");
        delete s;
    }

    void Renderer::BeginFrame()
    {
        ZPG_PROFILE_FUNCTION();

        s->m_RenderContext.Batches.Clear();
        s->m_RenderContext.Queues.Clear();
        s->m_RenderContext.Lights.Clear();
        s->m_RenderContext.ActiveSky = nullptr;
        s->m_RenderContext.ActiveCamera = nullptr;
        s->m_RenderContext.Statistics.Reset();
    }

    void AddCommands(vec<DrawCommand>& queue, umap<DrawBatchKey, DrawBatch>& batches, const vec<DrawCommand>& drawCommands)
    {
        for(auto& command : drawCommands)
        {
            queue.push_back(command);

            DrawBatchKey key{ .MeshId = command.mesh->GetID() };
            if (!batches.contains(key)) {
                batches[key] = DrawBatch{ .mesh = command.mesh };
            }
            batches[key].entityIDs.push_back(command.entityID);
            batches[key].transforms.push_back(command.transform);
        }
    };

    void Renderer::SetCamera(const Camera& camera)
    {
        ZPG_PROFILE_FUNCTION();
        s->m_RenderContext.ActiveCamera = (Camera*)&camera;
        s->m_RenderContext.SSBO.CameraSSBO.SetCamera(camera);
        s->m_RenderContext.ViewingFrustum.Set(camera);
    }

    void Renderer::SetLights(const std::vector<ref<Light>>& lights)
    {
        ZPG_PROFILE_FUNCTION();

        s->m_RenderContext.Lights.AmbientLight = nullptr;
        s->m_RenderContext.Lights.DirectionalLight = nullptr;
        s->m_RenderContext.Lights.PointLights.clear();
        s->m_RenderContext.Lights.SpotLights.clear();

        int spotLightIndex = 0;
        int pointLightIndex = 0;

        for (const auto& light : lights)
        {
            switch (light->GetLightType())
            {
                case LightType::Ambient:
                {
                    s->m_RenderContext.Lights.AmbientLight = new AmbientLightStruct(*(AmbientLight*)light.get());
                    s->m_RenderContext.SSBO.EnvironmentLightSSBO.SetAmbient(*(AmbientLight*)light.get());
                } break;
                case LightType::Directional:
                {
                    s->m_RenderContext.Lights.DirectionalLight = new DirectionalLightStruct(
                        *(DirectionalLight*)light.get(), 
                        *s->m_RenderContext.ActiveCamera
                    );
                    s->m_RenderContext.SSBO.EnvironmentLightSSBO.SetDirectionalLight(
                        *(DirectionalLight*)light.get(), 
                        *s->m_RenderContext.ActiveCamera
                    );
                } break;
                case LightType::Point:
                {
                    const auto& pointLights = s->m_RenderContext.Lights.PointLights;
                    if (pointLights.size() < pointLights.capacity())
                    {
                        PointLightStruct pointlightStruct(*(PointLight*)light.get());
                        pointlightStruct.Index = pointLightIndex++;
                        s->m_RenderContext.Lights.PointLights.push_back(pointlightStruct);
                    }
                } break;
                case LightType::Spotlight:
                {
                    const auto& spotLights = s->m_RenderContext.Lights.SpotLights;
                    if (spotLights.size() < spotLights.capacity())
                    {
                        SpotLightStruct spotlightStruct(*(SpotLight*)light.get());
                        spotlightStruct.Index = spotLightIndex++;
                        s->m_RenderContext.Lights.SpotLights.push_back(spotlightStruct);
                    }
                } break;
                case LightType::None:
                    ZPG_UNREACHABLE("LightType::None is not supported");
            }
        }

        s->m_RenderContext.SSBO.PointLightSSBO.SetLightArray(s->m_RenderContext.Lights.PointLights);
        s->m_RenderContext.SSBO.SpotLightSSBO.SetLightArray(s->m_RenderContext.Lights.SpotLights);
    }

    void Renderer::SetSky(const Sky* sky)
    {
        s->m_RenderContext.ActiveSky = (Sky*)sky;
    }

    void Renderer::Submit(const Entity& entity, RenderFlags flags)
    {
        ZPG_PROFILE_FUNCTION();

        ZPG_CORE_ASSERT(flags.IsValid());

        auto entityID = entity.GetEntityID();
        auto model = entity.GetModel();
        auto entityTransformMatrix = entity.GetTransformMatrix();

        s->m_RenderContext.Statistics.Submissions++;

        vec<DrawCommand> drawCommands;

        for (const auto& mesh : model->GetMeshes())
        {
            const auto& volume = mesh->GetVolume();
            Transform meshTransform(entityTransformMatrix * mesh->GetLocalTransform());
            float tolerance = 10.0f;  // less agressive culling, but distant objects don't have shadows
            if (!volume->IsInFrustum(s->m_RenderContext.ViewingFrustum, meshTransform, tolerance))
            {
                continue;
            }

            // v3 entityPosition = v3(command.transform[3]) / command.transform[3].w;
            // if (s->m_RenderContext.ViewingFrustum.ClassifyPoint(entityPosition) == FrustumHitOutside)
            //     continue;

            DrawCommand command;
            command.entityID = entityID;
            command.mesh = mesh.get();
            command.transform = meshTransform.GetMatrix();
            drawCommands.push_back(command);
        }

        if (flags & RenderFeatureCastsShadow)
        {
            ADD_COMMANDS(flags & RenderFeatureStatic, Shadow, drawCommands);
        }

        if (flags & RenderFeatureForward && flags & RenderFeatureTransparent)
        {
            ADD_COMMANDS(flags & RenderFeatureStatic, ForwardTransparent, drawCommands);
        }

        if (flags & RenderFeatureForward && flags & RenderFeatureOpaque)
        {
            ADD_COMMANDS(flags & RenderFeatureStatic, ForwardOpaque, drawCommands);
        }

        if (flags & RenderFeatureDeferred)
        {
            ADD_COMMANDS(flags & RenderFeatureStatic, GeometryBuffer, drawCommands);
        }
    }

    void Renderer::EndFrame()
    {
        ZPG_PROFILE_FUNCTION();


        for (const auto& renderPass : s->m_RenderPasses)
        {
            renderPass->Execute(s->m_RenderContext);
        }
    }

    void Renderer::RenderScene(const Scene& scene)
    {
        ZPG_PROFILE_FUNCTION();

        Renderer::BeginFrame();
        Renderer::SetCamera(scene.GetCamera());
        Renderer::SetLights(scene.GetLightManager().GetLights());
        Renderer::SetSky(scene.GetSky().get());

        for (const auto& entity : scene.GetEntityManager().GetEntities())
        {
            Renderer::Submit(*entity, RenderFeatureDeferred | RenderFeatureCastsShadow | RenderFeatureDynamic);
        }

        Renderer::EndFrame();
    }

    void Renderer::PushRenderPass(RenderPass* renderPass)
    {
        ZPG_PROFILE_FUNCTION();
        renderPass->Init(s->m_RenderContext);
        s->m_RenderPasses.push_back(renderPass);
    }

    const RenderContext& Renderer::GetRenderContext()
    {
        ZPG_PROFILE_FUNCTION();
        return s->m_RenderContext;
    }

    RenderContext& Renderer::GetRenderContextMut()
    {
        ZPG_PROFILE_FUNCTION();
        return s->m_RenderContext;
    }

    const RenderStatistics& Renderer::GetStats()
    {
        ZPG_PROFILE_FUNCTION();
        return s->m_RenderContext.Statistics;
    }

    void Renderer::Clear()
    {
        ZPG_PROFILE_FUNCTION();

        Renderer::BeginFrame();
        s->m_RenderContext.StaticBatches.Clear();
        s->m_RenderContext.StaticQueues.Clear();
        s->m_RenderContext.SSBO.PointLightSSBO.Null();
        s->m_RenderContext.SSBO.EnvironmentLightSSBO.Null();
        s->m_RenderContext.SSBO.SpotLightSSBO.Null();
        s->m_RenderContext.SSBO.EntitySSBO.Null();
        s->m_RenderContext.SSBO.CameraSSBO.Null();
        s->m_RenderContext.SSBO.ModelSSBO.Null();
        s->m_RenderContext.SSBO.MaterialSSBO.Null();
    }

    void Renderer::OnResize(u32 width, u32 height)
    {
        ZPG_PROFILE_FUNCTION();

        for (auto& renderPass : s->m_RenderPasses)
        {
            renderPass->OnResize(width, height);
        }
    }
}
