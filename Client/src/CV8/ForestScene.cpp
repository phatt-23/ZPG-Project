//
// Created by phatt on 11/3/25.
//

#include "ForestScene.h"

#include "Platform/OpenGL/OpenGLCore.h"
#include "Renderer/Renderer.h"

using namespace ZPG;

namespace CV8
{
    ForestScene::ForestScene()
    {
        ref<SpotLight> spotlight = MakeRef(new SpotLight(
            ColorComponent(v4(1.0)),
            PositionComponent(v3(0.0)),
            DirectionComponent(v3(0.0, 0.0, -1.0)),
            BeamShapeComponent(20.0, 0.5),
            AttenComponent(0.001, 0.001, 0.01)
        ));

        GetLightManager().AddLight(spotlight);

        FlashlightCameraController* flashlightCameraController = new FlashlightCameraController(spotlight, GetCamera());
        SetCameraController(MakeRef(flashlightCameraController));
    }

    void ForestScene::OnLazyAttach()
    {
        m_LocalRes.LoadModel("Tree", "./assets/models/vendor/pine_tree/scene.gltf");
        m_LocalRes.LoadModel("Bush", "./assets/models/vendor/bush/scene.gltf");
        m_LocalRes.LoadModel("Bush2", "./assets/models/vendor/small_bush/scene.gltf");
        m_LocalRes.LoadModel("Firefly", "./assets/models/sphere/sphere.gltf");
        m_LocalRes.LoadModel("GrassBlock", "./assets/models/vendor/minecraft_grass_block/scene.gltf");
        m_LocalRes.LoadModel("Shrek", "./assets/models/vendor/shrek/shrek.obj");
        m_LocalRes.LoadModel("Fiona", "./assets/models/vendor/shrek/fiona.obj");

        SetSky(Skybox::Create(SkyboxSpecification{ .Directory = "./assets/textures/basic-skybox/" }));

        for (auto& ffMesh : m_LocalRes.GetModel("Firefly")->GetMeshes()) {
            ffMesh->GetMaterial()->SetEmissive(v4(1.0));
        }


        // Add lights
        GetLightManager().AddLight(new AmbientLight(v4(1.0, 1.0, 1.0, 0.01)));
        GetLightManager().AddLight(new DirectionalLight(v4(1.0, 1.0, 1.0, 0.2), v3(-1, -1, -1)));

        f32 planeSize = 40;

        // Add trees
        for (int i = 0; i < 50; i++) {
            f32 x = planeSize * Utility::GetRandomFloat(-1, 1);
            f32 z = planeSize * Utility::GetRandomFloat(-1, 1);

            auto transform = TransformGroup::Build()
                             .Add<Rotate>(90, v3(1, 0, 0))
                             .Add<Scale>(0.06)
                             .Add<Translate>(x, 0.0, z)
                             .Compose();

            // GetEntityManager().AddStaticEntity(new Entity(m_LocalRes.GetModel("Tree"), transform));
            GetEntityManager().AddEntity(new Entity(m_LocalRes.GetModel("Tree"), transform));
        }

        // Add bushes
        for (int i = 0; i < 50; i++) {
            f32 x = planeSize * Utility::GetRandomFloat(-1, 1);
            f32 z = planeSize * Utility::GetRandomFloat(-1, 1);

            auto transform = TransformGroup::Build()
                             .Add<Rotate>(-90, v3(1, 0, 0))
                             .Add<Scale>(2.0)
                             .Add<Translate>(x, 0.0, z)
                             .Compose();

            // GetEntityManager().AddStaticEntity(
            //     new Entity(m_LocalRes.GetModel(i % 2 == 0 ? "Bush" : "Bush2"), transform));
            GetEntityManager().AddEntity(
                new Entity(m_LocalRes.GetModel(i % 2 == 0 ? "Bush" : "Bush2"), transform));
        }

        // Add fireflies
        for (int i = 0; i < 20; i++) {
            f32 x = planeSize * Utility::GetRandomFloat(-1, 1);
            f32 y = planeSize * Utility::GetRandomFloat(0.01, 0.3);
            f32 z = planeSize * Utility::GetRandomFloat(-1, 1);

            auto transform = TransformGroup::Build()
                             .Add<Scale>(0.1)
                             .Add<Translate>(1.0f/planeSize * v3(x, y, z))
                             .Add<DynRotate>(0, 20.0)
                             .Add<Translate>(x, y, z)
                             .Add<DynRotate>(0, 20.0, v3(x, 4 * planeSize, z))
                             .Compose();

            auto pointLight = MakeRef(new PointLight(
                v4(1.0),
                v3(0.0),
                AttenComponent(0.2, 0.4)
            ));
            GetLightManager().AddLight(pointLight);

            GetEntityManager().AddEntity(
                new PointLightEntity(pointLight, m_LocalRes.GetModel("Firefly"), transform));
        }

        // Add ground
        int groundSize = 10;
        float groundDist = 12.0;

        for (int i = -groundSize; i < groundSize; i++) {
            for (int j = -groundSize; j < groundSize; j++) {
                auto transform = TransformGroup::Build()
                                 .Add<Scale>(groundDist * 0.5)
                                 .Add<Rotate>(90.0, v3(1, 0, 0))
                                 .Add<Translate>(v3(groundDist * i, -groundDist, groundDist * j))
                                 .Compose();

                // GetEntityManager().AddStaticEntity(
                //     new Entity(m_LocalRes.GetModel("GrassBlock"), transform));
                GetEntityManager().AddEntity(
                    new Entity(m_LocalRes.GetModel("GrassBlock"), transform));
            }
        }


        // Shrek and Fiona
        for (int i = 0; i < 10; i++) {
            f32 x = planeSize * Utility::GetRandomFloat(-1, 1);
            f32 z = planeSize * Utility::GetRandomFloat(-1, 1);

            auto transform = TransformGroup::Build()
                             .Add<Scale>(10.0)
                             .Add<Translate>(x, 0.0, z)
                             .Compose();

            GetEntityManager().AddEntity(
                new Entity(m_LocalRes.GetModel(i % 2 ? "Shrek" : "Fiona"), transform));
        }
    }

    void ForestScene::OnEvent(Event& event)
    {
        Scene::OnEvent(event);

        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MouseButtonPressedEvent>(ZPG_FORWARD_EVENT_TO_MEMBER_FN(ForestScene::OnMouseButtonPressed));
    }


    bool ForestScene::OnMouseButtonPressed(MouseButtonPressedEvent& event)
    {
        auto& mainFrameBuffer = Renderer::GetRenderContext().Targets.MainFrameBuffer;
        auto& sceneViewportInfo = Application::Get().GetSceneViewportInfo();

        v2 mouse = sceneViewportInfo.MousePosition;
        i32 entityID = mainFrameBuffer->ReadPixelInt(mouse.x, mouse.y, FrameBufferAttachmentType::Color, 1);
        f32 depth = mainFrameBuffer->ReadPixelFloat(mouse.x, mouse.y, FrameBufferAttachmentType::Depth, 0);

        if (event.GetButtonCode() == ZPG_MOUSE_BUTTON_LEFT)
        {
            // ZPG_INFO("Removing Entity ID: {0}", entityID);
            // GetEntityManager().RemoveEntity(entityID);

            ZPG_INFO("Clicked on Entity ID: {0}", entityID);
            auto entity = GetEntityManager().GetEntity(entityID);
            if (entity != nullptr)
            {
                const m4& tr = entity->GetTransformMatrix();
                v3 pos = v3(tr[3]) / tr[3].w;
                ZPG_INFO("World Position: {0}, {1}, {2}", pos.x, pos.y, pos.z);
                
                if (Renderer::GetRenderContext().ViewingFrustum.IsPointInside(pos) == FrustumHitOutside)
                {
                    ZPG_INFO("Object OUTSIDE frustum");
                }
                else 
                {
                    ZPG_INFO("Object INSIDE frustum");
                }
            }

        }
        else if (event.GetButtonCode() == ZPG_MOUSE_BUTTON_RIGHT)
        {
            v3 pos = glm::unProject(
                v3(mouse.x, mouse.y, depth),
                GetCamera().GetViewMatrix(),
                GetCamera().GetProjMatrix(),
                v4(0, 0, sceneViewportInfo.PanelSize.x, sceneViewportInfo.PanelSize.y)
            );

            ZPG_INFO("Mouse Position: {0}, {1}, Depth: {2}", mouse.x, mouse.y, depth);
            ZPG_INFO("World Position: {0}, {1}, {2}", pos.x, pos.y, pos.z);

            Entity* entity = new Entity(
                m_LocalRes.GetModel("Tree"),
                TransformGroup::Build()
                    .Add<Rotate>(90, v3(1, 0, 0))
                    .Add<Scale>(0.06)
                    .Add<Translate>(pos)
                    .Compose()
            );

            GetEntityManager().AddEntity(entity);
        }

        return false;
    }
}
