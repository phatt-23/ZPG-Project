//
// Created by phatt on 11/3/25.
//

#include "ForestScene.h"

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
            AttenComponent(0.0001, 0.0001, 0.1)
        ));

        GetLightManager().AddLight(spotlight);

        FlashlightCameraController* flashlightCameraController = new FlashlightCameraController(spotlight, GetCamera());
        SetCameraController(MakeRef(flashlightCameraController));
    }

    void ForestScene::OnLazyAttach() {
        m_LocalRes.LoadModel("Tree", "./assets/models/pine_tree/scene.gltf");
        m_LocalRes.LoadModel("Bush", "./assets/models/bush/scene.gltf");
        m_LocalRes.LoadModel("Bush2", "./assets/models/small_bush/scene.gltf");
        m_LocalRes.LoadModel("Firefly", "./assets/models/sphere/scene.gltf");
        m_LocalRes.LoadModel("GrassBlock", "./assets/models/minecraft_grass_block/scene.gltf");
        m_LocalRes.LoadModel("Shrek", "./assets/models/shrek/shrek.obj");
        m_LocalRes.LoadModel("Fiona", "./assets/models/shrek/fiona.obj");


        for (auto& ffMesh : m_LocalRes.GetModel("Firefly")->GetMeshes()) {
            ffMesh->GetMaterial()->SetEmissive(v4(1.0));
        }


        // Add lights
        GetLightManager().AddLight(new AmbientLight(v4(1.0, 1.0, 1.0, 0.1)));
        GetLightManager().AddLight(new DirectionalLight(v4(1.0, 1.0, 1.0, 0.5), v3(-1, -1, -1)));

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

            GetEntityManager().AddEntity(new Entity(m_LocalRes.GetModel("Tree"), transform));
        }

        // Add bushes
        for (int i = 0; i < 100; i++) {
            f32 x = planeSize * Utility::GetRandomFloat(-1, 1);
            f32 z = planeSize * Utility::GetRandomFloat(-1, 1);

            auto transform = TransformGroup::Build()
                             .Add<Rotate>(-90, v3(1, 0, 0))
                             .Add<Scale>(2.0)
                             .Add<Translate>(x, 0.0, z)
                             .Compose();

            GetEntityManager().AddEntity(
                new Entity(m_LocalRes.GetModel(i % 2 == 0 ? "Bush" : "Bush2"), transform));
        }

        // Add fireflies
        for (int i = 0; i < 50; i++) {
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
        int groundSize = 4;
        float groundDist = 12.0;

        for (int i = -groundSize; i < groundSize; i++) {
            for (int j = -groundSize; j < groundSize; j++) {
                auto transform = TransformGroup::Build()
                                 .Add<Scale>(groundDist * 0.5)
                                 .Add<Rotate>(90.0, v3(1, 0, 0))
                                 .Add<Translate>(v3(groundDist * i, -groundDist, groundDist * j))
                                 .Compose();

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
}
