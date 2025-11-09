#include "MultipassRenderer.h"
#include "Debug/Asserter.h"
#include "Renderer/RenderPass.h"
#include "Renderer/RenderPass/DirectionalLightShadowRenderPass.h"
#include "Renderer/RenderPass/PointLightShadowRenderPass.h"
#include "Renderer/RenderPass/SpotLightShadowRenderPass.h"
#include "Renderer/RenderPass/GeometryRenderPass.h"
#include "Renderer/RenderPass/LightingRenderPass.h"
#include "Entity/Entity.h"
#include "Model/Model.h"
#include "Model/Mesh.h"
#include "Profiling/Instrumentor.h"
#include "Scene/Scene.h"


namespace ZPG
{
    void MultipassRenderer::Init()
    {
        ZPG_PROFILE_FUNCTION();

        ZPG_CORE_ASSERT(s_Instance == nullptr, "MultipassRenderer already initialized.");
       
        RenderContextSpecification renderContextSpec;
        renderContextSpec.BatchSize = 2 * 1024;
        renderContextSpec.PointLightCapacity = 256;
        renderContextSpec.SpotLightCapacity = 256;

        s_Instance = new MultipassRenderer(renderContextSpec);

        PushRenderPass(new DirectionalLightShadowRenderPass());
        // PushRenderPass(new PointLightShadowRenderPass());
        PushRenderPass(new SpotLightShadowRenderPass());
        PushRenderPass(new GeometryRenderPass());
        PushRenderPass(new LightingRenderPass());
    }

    void MultipassRenderer::Shutdown()
    {
        ZPG_PROFILE_FUNCTION();

        ZPG_CORE_ASSERT(s_Instance != nullptr, "MultipassRenderer wasn't initialized.");

        delete s_Instance;
    }

    void MultipassRenderer::PushRenderPass(RenderPass* renderPass)
    {
        ZPG_PROFILE_FUNCTION();

        ZPG_CORE_ASSERT(s_Instance);
        s_Instance->PushRenderPassImpl(renderPass);
    }

    void MultipassRenderer::RenderScene(const Scene& scene)
    {
        ZPG_PROFILE_FUNCTION();

        ZPG_CORE_ASSERT(s_Instance);
        s_Instance->RenderSceneImpl(scene);
    }

    void MultipassRenderer::OnResize(u32 width, u32 height)
    {
        ZPG_PROFILE_FUNCTION();

        ZPG_CORE_ASSERT(s_Instance);
        s_Instance->OnResizeImpl(width, height);
    }

    const RenderContext& MultipassRenderer::GetRenderContext()
    {
        ZPG_PROFILE_FUNCTION();

        ZPG_CORE_ASSERT(s_Instance);
        return s_Instance->m_RenderContext;
    }

    const RenderStatistics& MultipassRenderer::GetStats()
    {
        ZPG_PROFILE_FUNCTION();

        ZPG_CORE_ASSERT(s_Instance);
        return s_Instance->m_RenderContext.Statistics;
    }

    MultipassRenderer::MultipassRenderer(const RenderContextSpecification& renderContextSpecification)
        : m_RenderContext(renderContextSpecification)
    {   
    }

    MultipassRenderer::~MultipassRenderer()
    {
    }

    void MultipassRenderer::RenderSceneImpl(const Scene& scene)
    {
        ZPG_PROFILE_FUNCTION();

        m_RenderContext.Statistics.Reset();
        SetCameraImpl(scene.GetCamera());
        SetLightsImpl(scene.GetLightManager().GetLights());
        m_RenderContext.VisibleEntities = scene.GetEntityManager().GetEntities(); // todo: cull here
        m_RenderContext.ActiveSky = scene.GetSky();

        for (auto& renderPass : m_RenderPasses) 
        {
            renderPass->Execute(m_RenderContext);
        }
    }

    void MultipassRenderer::PushRenderPassImpl(RenderPass* renderPass)
    {
        ZPG_PROFILE_FUNCTION();

        renderPass->Init(m_RenderContext);
        m_RenderPasses.push_back(renderPass);
    }

    void MultipassRenderer::OnResizeImpl(u32 width, u32 height)
    {
        ZPG_PROFILE_FUNCTION();

        for (auto& renderPass : m_RenderPasses)
        {
            renderPass->OnResize(width, height);
        }

        m_RenderContext.MainFramebuffer->Resize(width, height);
        m_RenderContext.GeometryPassFramebuffer->Resize(width, height);
        // m_RenderContext.DirectionalLightShadowFramebuffer->Resize(width, height);
        // m_RenderContext.PointLightShadowFramebuffer->Resize(width, height);
        // m_RenderContext.SpotLightShadowFramebuffer->Resize(width, height);
    }

    void MultipassRenderer::SetCameraImpl(const Camera& camera)
    {
        ZPG_PROFILE_FUNCTION();

        m_RenderContext.ActiveCamera = (Camera*)&camera;
        m_RenderContext.CameraSSBO.SetLayout(camera);
    }

    void MultipassRenderer::SetLightsImpl(const std::vector<ref<Light>>& lights)
    {
        ZPG_PROFILE_FUNCTION();

        m_RenderContext.AmbientLight = nullptr;
        m_RenderContext.DirectionalLight = nullptr;
        m_RenderContext.PointLights.clear();
        m_RenderContext.SpotLights.clear();

        int spotLightShadowLayer = 0;

        for (const auto& light : lights)
        {
            switch (light->GetLightType())
            {
                case LightType::Ambient: 
                    m_RenderContext.AmbientLight = new AmbientLightStruct(*(AmbientLight*)light.get());
                    m_RenderContext.EnvironmentLightSSBO.SetAmbient(*(AmbientLight*)light.get());
                    break;
                case LightType::Directional:
                    m_RenderContext.DirectionalLight = new DirectionalLightStruct(*(DirectionalLight*)light.get(), *m_RenderContext.ActiveCamera);
                    m_RenderContext.EnvironmentLightSSBO.SetDirectionalLight(*(DirectionalLight*)light.get(), *m_RenderContext.ActiveCamera);
                    break;
                case LightType::Point: 
                    if (m_RenderContext.PointLights.size() < m_RenderContext.PointLights.capacity())
                    {
                        m_RenderContext.PointLights.push_back(PointLightStruct(*(PointLight*)light.get()));
                    }
                    break;
                case LightType::Spotlight: 
                    if (m_RenderContext.SpotLights.size() < m_RenderContext.SpotLights.capacity())
                    {
                        SpotLightStruct spotlightStruct(*(SpotLight*)light.get());
                        spotlightStruct.ShadowLayer = spotLightShadowLayer++;
                        m_RenderContext.SpotLights.push_back(spotlightStruct);
                    }
                    break;
                case LightType::None:
                    ZPG_UNREACHABLE("LightType::None is not supported");
            }
        }

        m_RenderContext.PointLightSSBO.SetLightArray(m_RenderContext.PointLights);
        m_RenderContext.SpotLightSSBO.SetLightArray(m_RenderContext.SpotLights);
    }
}



