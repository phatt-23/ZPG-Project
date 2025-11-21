#include "Transform/MovementTransform/LineMovement.h"
#include "ZPGine.h"
#include <chrono>

namespace CV10
{
    using namespace ZPG;

    class WhackScene : public Scene
    {
    public:

        struct Mole
        {
            ref<Entity> entity;
            ref<TransformGroup> transform;
            int loot = 100;    // points awarded on click
        };

        struct PendingRemoval
        {
            int entityID;
            std::chrono::steady_clock::time_point removeAt;
        };


        void OnAttach() override
        {
            GetLightManager().AddLight(new AmbientLight(v4(0.1)));
            GetLightManager().AddLight(new DirectionalLight(v4(1.0, 1.0, 1.0, 0.8), v3(-1,-1,-1)));

            SetSky(Skybox::Create({ .Directory = "./assets/textures/basic-skybox/" }));

            GetCamera().SetPosition(v3(0.0, 2.0, 0.0));

            // ground
            GetEntityManager().AddStaticEntity(new Entity(
                GetResourceManager().GetModel(CommonResources::MODEL_PLANE),
                TransformGroup::Build()
                    .Add<Scale>(60.0)
                    .Compose()
            ));

            // models
            m_Res.LoadModel("Shrek", "./assets/models/vendor/shrek/shrek.obj");
            m_Res.LoadModel("Fiona", "./assets/models/vendor/shrek/fiona.obj");
        }


        void OnEvent(Event &event) override
        {
            Scene::OnEvent(event);
            EventDispatcher dispatcher(event);
            dispatcher.Dispatch<MouseButtonPressedEvent>(ZPG_FORWARD_EVENT_TO_MEMBER_FN(OnMouseButtonPressed));
        }


        bool OnMouseButtonPressed(MouseButtonPressedEvent& event)
        {
            if (event.GetButtonCode() != ZPG_MOUSE_BUTTON_LEFT)
            {
                return false;
            }

            auto& mainFrameBuffer = Renderer::GetRenderContext().Targets.MainFrameBuffer;
            auto& sceneViewportInfo = Application::Get().GetSceneViewportInfo();

            v2 mouse = sceneViewportInfo.MousePosition;

            i32 entityID = mainFrameBuffer->ReadPixelInt(mouse.x, mouse.y, FrameBufferAttachmentType::Color, 1);

            std::scoped_lock lock(m_Mutex);

            auto it = std::ranges::find_if(m_Moles, [entityID](const Mole& m)
            {
                return m.entity->GetEntityID() == entityID;
            });

            if (it == m_Moles.end())
            {
                return false;
            }

            Mole& mole = *it;

            // award points
            m_GameState.Score += mole.loot;
            mole.loot = 0;

            // animation
            float duration = 1.0f;
            mole.transform->Add<LineMovement>(v3(0.0f), v3(0, -5, 0), duration, MovementMode::Once);

            // schedule removal
            PendingRemoval pr;
            pr.entityID = entityID;
            pr.removeAt = std::chrono::steady_clock::now() + std::chrono::milliseconds((int)(duration * 1000));
            m_PendingRemovals.push_back(pr);

            return false;
        }


        void OnImGuiRender() override
        {
            Scene::OnImGuiRender();
            
            ImGui::Begin("Game State");
            ImGui::Text("Score: %d", m_GameState.Score);
            ImGui::End();
        }

        void OnUpdate(Timestep &ts) override
        {
            {
                std::scoped_lock lock(m_Mutex);

                // Spawn new moles
                if (m_Moles.size() < m_MaxMoleCount)
                {
                    float x = (Utility::GetRandomFloat() - 0.5f) * 10.0f;
                    float z = (Utility::GetRandomFloat() - 0.5f) * 10.0f;

                    auto transform = TransformGroup::Build()
                        .Add<LineMovement>(v3(x, -3.0f, z), v3(x, 2.0f, z), 0.6f, MovementMode::Once)
                        .Compose();

                    Mole mole;
                    mole.transform = transform;
                    static int choice = 0;
                    mole.entity = MakeRef(new Entity(m_Res.GetModel((choice++ % 2) ? "Shrek" : "Fiona"), transform));
                    mole.loot = 100;

                    m_Moles.push_back(std::move(mole));
                }

                // handle scheduled removals
                auto now = std::chrono::steady_clock::now();
                for (auto it = m_PendingRemovals.begin(); 
                     it != m_PendingRemovals.end();
                )
                {
                    if (it->removeAt <= now)
                    {
                        int id = it->entityID;

                        auto found = std::ranges::find_if(
                            m_Moles, [id](const Mole& m)
                            {
                                return m.entity->GetEntityID() == id;
                            }
                        );

                        if (found != m_Moles.end())
                        {
                            m_Moles.erase(found);
                        }

                        it = m_PendingRemovals.erase(it);
                    }
                    else
                    {
                        ++it;
                    }
                }
            }

            // update objects
            for (auto& e : GetEntityManager().GetEntities())
            {
                e->Update(ts);
            }

            for (auto& mole : m_Moles)
            {
                mole.entity->Update(ts);
            }

            if (Application::Get().GetSceneViewportInfo().IsActive())
            {
                GetCameraController()->OnUpdate(ts);
            }
        }

        void OnRender() override
        {
            Renderer::BeginFrame();
            Renderer::SetCamera(GetCamera());
            Renderer::SetLights(GetLightManager().GetLights());
            Renderer::SetSky(GetSky().get());

            for (const auto& e : GetEntityManager().GetEntities())
            {
                Renderer::Submit(*e, RenderFeatureDeferred | RenderFeatureCastsShadow | RenderFeatureDynamic);
            }

            for (const auto& mole : m_Moles)
            {
                Renderer::Submit(*mole.entity, RenderFeatureDeferred | RenderFeatureCastsShadow | RenderFeatureDynamic);
            }

            Renderer::EndFrame();
        }

    private:
        ResourceManager m_Res;

        std::mutex m_Mutex;

        std::vector<Mole> m_Moles;
        std::vector<PendingRemoval> m_PendingRemovals;

        struct GameState 
        { 
            int Score = 0; 
        } m_GameState;

        int m_MaxMoleCount = 10;
    };
}

