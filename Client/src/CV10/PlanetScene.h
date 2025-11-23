#include "Resource/CommonResources.h"
#include "Resource/ResourceManager.h"
#include "Sky/Skybox.h"
#include "ZPGine.h"

namespace CV10 
{
    using namespace ZPG;

    class PlanetScene : public Scene
    {
    public:        
        void OnAttach() override
        {
            SetSky(Skybox::Create({ 
                .Directory = "./assets/textures/stars-skybox/", 
                .Filenames = {
                    "right.png",   
                    "left.png",    
                    "top.png",     
                    "bottom.png",  
                    "front.png",   
                    "back.png"     
                },
                .DataFormat = TextureDataFormat::RGBA8,
            }));

            m_Res.LoadModel("Sun", "./assets/models/sphere/sphere.gltf");
            m_Res.LoadModel("Earth", "./assets/models/sphere/sphere.gltf");
            m_Res.LoadModel("Moon", "./assets/models/sphere/sphere.gltf");

            m_Res.GetModel("Earth")->GetMeshes().front()->GetMaterial()->SetAlbedoMap(
                Texture2D::Create("./assets/textures/planets/2k_earth_daymap.jpg")
            );
            m_Res.GetModel("Sun")->GetMeshes().front()->GetMaterial()->SetAlbedoMap(
                Texture2D::Create("./assets/textures/planets/2k_sun.jpg")
            );
            m_Res.GetModel("Moon")->GetMeshes().front()->GetMaterial()->SetAlbedoMap(
                Texture2D::Create("./assets/textures/planets/2k_moon.jpg")
            );

            m_Res.GetModel("Sun")->GetMeshes().front()->GetMaterial()->SetEmissive(v4(1.0));
            m_Res.GetModel("Sun")->GetMeshes().front()->GetMaterial()->SetMetallic(0.0);
            m_Res.GetModel("Earth")->GetMeshes().front()->GetMaterial()->SetMetallic(0.0);
            m_Res.GetModel("Moon")->GetMeshes().front()->GetMaterial()->SetMetallic(0.0);

            auto sun = new Entity(
                m_Res.GetModel("Sun"),
                TransformGroup::Build()
                    .Add<Scale>(2.0f)
                    // full rotation in 27 days
                    .Add<DynRotate>(0.0f, 360.f/27.f, v3(0,1,0))  
                    .Compose()
            );

            auto earth = new Entity(
                m_Res.GetModel("Earth"),
                TransformGroup::Build()
                    .WithParent(sun->GetTransform())
                    // full rotation in 1 day
                    .Add<DynRotate>(0.0f, 360.f/1.f, v3(0,0.9,0.2))  
                    // full orbit in 365 days
                    .Add<CircleMovement>(v3(0.0f), 5.0f, v3(0,1,0), 365, MovementMode::Repeat)  
                    .Compose()
            );

            auto moon = new Entity(
                m_Res.GetModel("Moon"),
                TransformGroup::Build()
                    .WithParent(earth->GetTransform())
                    .Add<Scale>(0.4f)
                    // rotates around its axis in 27 days
                    .Add<DynRotate>(0.0f, -360.f/27.f, v3(0,1,0))  
                    // full orbit in 27 days
                    .Add<CircleMovement>(v3(0.0f), 2.0f, v3(0,1,0), 27.f, MovementMode::Repeat) 
                    .Compose()
            );

            GetEntityManager().AddEntity(sun);
            GetEntityManager().AddEntity(earth);
            GetEntityManager().AddEntity(moon);

            GetLightManager().AddLight(new AmbientLight(v4(1.0, 1.0, 1.0, 0.1)));
            GetLightManager().AddLight(new PointLight(v4(1.0), v3(0.0), AttenComponent(0.001, 0.001, 0.01)));
        }

        void OnUpdate(Timestep& ts) override
        {
            Timestep scaledTs = ts.AsSeconds() * 1.0f / m_TimeScale;
            for (auto& entity : GetEntityManager().GetEntities())
            {
                entity->Update(scaledTs);
            }

            if (Application::Get().GetSceneViewportInfo().IsActive())
            {
                GetCameraController()->OnUpdate(ts);
            }
        }

        void OnImGuiRender() override
        {
            Scene::OnImGuiRender();
            ImGui::Begin("Planet Scene");
            ImGui::DragFloat("Speed", &m_TimeScale, 0.01f);
            ImGui::End();
        }

    private:
        float m_TimeScale = 1.0f;
        ResourceManager m_Res;
    };
}
