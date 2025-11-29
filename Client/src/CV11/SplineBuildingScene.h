#pragma once

#include "ZPGine.h"

namespace CV11
{
    using namespace ZPG;

    class SplineBuildingScene : public Scene
    {
    public:
        void OnAttach() override
        {
            m_Resource.LoadModel("Formula", "./assets/models/nemec-assets/formula1.obj");

            auto formulaMaterial = MakeRef<Material>();
            formulaMaterial->SetAlbedo({1,0,0,1});  
            formulaMaterial->SetRoughness(0);        
            formulaMaterial->SetMetallic(0);

            m_Resource.GetModel("Formula")->GetMeshes().front()->SetMaterial(formulaMaterial);

            m_FormulaBaseTransform = TransformGroup::Build()
                .Add<Rotate>(90.0f, v3(0,1,0))
                .Add<Scale>(0.25)
                .Compose();

            GetLightManager().AddLight(new AmbientLight(v4(0.1)));
            GetLightManager().AddLight(new DirectionalLight(v4(1.0, 1.0, 1.0, 0.8), v3(-1,-1,-1)));

            SetSky(Skybox::Create(SkyboxSpecification{ .Directory = "./assets/textures/basic-skybox/" }));

            GetCamera().SetPosition(v3(0.0, 2.0, 0.0));

            GetEntityManager().AddEntity(new Entity(
                GetResourceManager().GetModel(CommonResources::MODEL_PLANE),
                TransformGroup::Build()
                    .Add<Scale>(100.0)
                    .Compose()
            ));
        }

        void OnEvent(Event& event) override
        {
            Scene::OnEvent(event);

            EventDispatcher dispatcher(event);
            dispatcher.Dispatch<MouseButtonPressedEvent>(ZPG_FORWARD_EVENT_TO_MEMBER_FN(SplineBuildingScene::OnMouseButtonPressed));
            dispatcher.Dispatch<KeyPressedEvent>(ZPG_FORWARD_EVENT_TO_MEMBER_FN(SplineBuildingScene::OnKeyPressed));
        }

        void InvalidateTracingEntity() 
        {
            if (m_SplinePoints.empty() && m_TracingEntity != nullptr)
            {
                GetEntityManager().RemoveEntity(m_TracingEntity->GetEntityID());
            }

            if (m_SplinePoints.size() % 4 == 0 && !m_SplinePoints.empty())
            {
                int segmentCount = m_SplinePoints.size() / 4;
                vec<SplineSegment> segments;

                for (int i = 0; i < segmentCount; i++)
                {
                    int base = 4 * i;
                    segments.emplace_back<SplineSegment>({
                        m_SplinePoints[base + 0].second, 
                        m_SplinePoints[base + 1].second, 
                        m_SplinePoints[base + 2].second, 
                        m_SplinePoints[base + 3].second, 
                    });
                }

                if (m_TracingEntity != nullptr)
                {
                    GetEntityManager().RemoveEntity(m_TracingEntity->GetEntityID());
                }

                m_TracingEntity = MakeRef<Entity>(
                    m_Resource.GetModel("Formula"),
                    TransformGroup::Build() 
                        .Include(m_FormulaBaseTransform)
                        .Add<SplineMovement>(
                            v3(0,0,0),
                            segments,
                            segments.size() * 1.f, 
                            MovementMode::PingPong)
                        .Compose()
                );

                GetEntityManager().AddEntity(m_TracingEntity);
            }
        }

        bool OnKeyPressed(KeyPressedEvent& event)
        {
            if (!Application::Get().GetSceneViewportInfo().IsActive()) {
                return false;
            }

            switch (event.GetKeyCode())
            {
                case ZPG_KEY_DELETE:
                {
                    m_SplineAnchorCubes.Clear();
                    m_SplinePoints.clear();
                } break;
                case ZPG_KEY_Z:
                {
                    if (event.IsMod(ZPG_MOD_ALT))
                    {
                        if (!m_SplinePoints.empty())
                        {
                            auto& [entityID, point] = m_SplinePoints.back();
                            m_SplineAnchorCubes.RemoveEntity(m_SplineAnchorCubes.GetEntities().back()->GetEntityID());
                            m_SplinePoints.pop_back();
                        }

                        InvalidateTracingEntity();
                    }
                } break;
                default:
                    break;
            }
            return false;
        }

        bool OnMouseButtonPressed(MouseButtonPressedEvent& event)
        {
            if (!Application::Get().GetSceneViewportInfo().IsActive()) {
                return false;
            }

            auto& renderContext = Renderer::GetRenderContext();
            auto& viewportInfo = Application::Get().GetSceneViewportInfo();

            v2 mouse = viewportInfo.MousePosition;
            float depth = renderContext.Targets.MainFrameBuffer->ReadPixelFloat(mouse.x, mouse.y, FrameBufferAttachmentType::Depth, 0);
            int entityID = renderContext.Targets.MainFrameBuffer->ReadPixelInt(mouse.x, mouse.y, FrameBufferAttachmentType::Color, 1);

            v3 worldPos = glm::unProject(
                v3(mouse.x, mouse.y, depth),
                GetCamera().GetViewMatrix(),
                GetCamera().GetProjMatrix(),
                v4(0, 0, viewportInfo.PanelSize.x, viewportInfo.PanelSize.y)
            );

            switch (event.GetButtonCode())
            {
                case ZPG_MOUSE_BUTTON_RIGHT:
                {
                    auto anchorCube = MakeRef<Entity>(
                        GetResourceManager().GetModel(CommonResources::MODEL_CUBE),
                        TransformGroup::Build() 
                            .Add<Scale>(0.5f)
                            .Add<Translate>(worldPos)
                            .Compose()
                    );

                    m_SplineAnchorCubes.AddEntity(anchorCube);
                    m_SplinePoints.push_back({anchorCube->GetEntityID(), worldPos});
                    InvalidateTracingEntity();
                } break;
                case ZPG_MOUSE_BUTTON_LEFT:
                {
                    m_SplineAnchorCubes.RemoveEntity(entityID);

                    std::erase_if(m_SplinePoints, [entityID](const std::pair<int, v3>& pair)
                    {
                        return pair.first == entityID;
                    });
                    InvalidateTracingEntity();
                } break;
                default:
                    break;
            }

            ZPG_INFO("Spline anchors count: {}", m_SplinePoints.size());

            return false;
        }

        void OnRender() override
        {
            Renderer::BeginFrame();
            Renderer::SetCamera(GetCamera());
            Renderer::SetLights(GetLightManager().GetLights());
            Renderer::SetSky(GetSky().get());

            for (const auto& entity : GetEntityManager().GetEntities())
            {
                Renderer::Submit(*entity, RenderFeatureDeferred | RenderFeatureCastsShadow | RenderFeatureDynamic);
            }

            for (const auto& entity : m_SplineAnchorCubes.GetEntities())
            {
                Renderer::Submit(*entity, RenderFeatureDeferred | RenderFeatureCastsShadow | RenderFeatureDynamic);
            }

            Renderer::EndFrame();
        }

    private:
        ResourceManager m_Resource;
        ref<Transform> m_FormulaBaseTransform;
        
        vec<std::pair<int, v3>> m_SplinePoints;

        EntityManager m_SplineAnchorCubes;
        ref<Entity> m_TracingEntity = nullptr;
    };

}
