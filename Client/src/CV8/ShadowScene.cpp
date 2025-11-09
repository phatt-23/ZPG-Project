//
// Created by phatt on 11/8/25.
//

#include "ShadowScene.h"

#include "Camera/FlashlightCameraController.h"
#include "Core/KeyCodes.h"
#include "Entity/Entity.h"
#include "Entity/SpotLightEntity.h"
#include "Event/Event.h"
#include "Event/KeyEvent.h"
#include "Light/AmbientLight.h"
#include "Light/DirectionalLight.h"
#include "Light/SpotLight.h"
#include "Material/Material.h"
#include "Model/Model.h"
#include "Transform/TransformGroup.h"
#include "Transform/StaticTransform/Scale.h"
#include "Transform/StaticTransform/Translate.h"

using namespace ZPG;

namespace CV8
{

    ShadowScene::ShadowScene()
    {

    }


    void ShadowScene::OnAttach()
    {
        auto spotlight = MakeRef(new SpotLight(v4(1.0), v3(0.0), v3(1.0), 20.f, 0.5f, AttenComponent(0.001, 0.001, 0.1f)));
        auto controller = MakeRef(new FlashlightCameraController(spotlight, GetCamera()));
        SetCameraController(controller);

        GetLightManager().AddLight(spotlight);

        GetLightManager().AddLight(new AmbientLight(v4(0.1)));
        GetLightManager().AddLight(new DirectionalLight(v4(1.0), v3(-1.0, -1.0, -1.0)));

        m_LocalResources.LoadModel("Plane", "./assets/models/plane.gltf");
        m_LocalResources.LoadModel("Sphere", "./assets/models/sphere.gltf");
        m_LocalResources.LoadModel("LightSphere", "./assets/models/sphere.gltf");
        m_LocalResources.LoadModel("Cube", "./assets/models/cube.gltf");
        m_LocalResources.LoadModel("GreenCube", "./assets/models/cube.gltf");
        m_LocalResources.LoadModel("BlueCube", "./assets/models/cube.gltf");
        m_LocalResources.LoadModel("RedCube", "./assets/models/cube.gltf");

        m_LocalResources.GetModel("Sphere")->GetMeshes().front()->GetMaterial()->SetAlbedo(v4(1.0, 1.0, 0.0, 1.0));
        m_LocalResources.GetModel("LightSphere")->GetMeshes().front()->GetMaterial()->SetAlbedo(v4(1.0, 1.0, 0.0, 1.0));
        m_LocalResources.GetModel("LightSphere")->GetMeshes().front()->GetMaterial()->SetEmissive(v4(1.0, 1.0, 0.0, 1.0));
        m_LocalResources.GetModel("RedCube")->GetMeshes().front()->GetMaterial()->SetAlbedo(v4(1.0, 0.0, 0.0, 1.0));
        m_LocalResources.GetModel("GreenCube")->GetMeshes().front()->GetMaterial()->SetAlbedo(v4(0.0, 1.0, 0.0, 1.0));
        m_LocalResources.GetModel("BlueCube")->GetMeshes().front()->GetMaterial()->SetAlbedo(v4(0.0, 0.0, 1.0, 1.0));

        {
            spotlight = MakeRef(new SpotLight(v4(1.0), v3(0.0), v3(0.0, -0.8, -1.0), 20.f, 0.5f, AttenComponent(0.001, 0.001, 0.1f)));
            GetLightManager().AddLight(spotlight);

            auto transform = TransformGroup::Build()
                .Add<Scale>(0.1)
                .Add<Translate>(5.0, 3.0, 5.0)
                .Compose();

            GetEntityManager().AddEntity(new SpotLightEntity(spotlight, m_LocalResources.GetModel("LightSphere"), transform));

            spotlight = MakeRef(new SpotLight(v4(1.0), v3(0.0), v3(0.0, -1.0, 0.0), 20.f, 0.5f, AttenComponent(0.001, 0.001, 0.1f)));
            GetLightManager().AddLight(spotlight);

            transform = TransformGroup::Build()
                .Add<Scale>(0.1)
                .Add<Translate>(0.0, 4.0, 0.0)
                .Compose();

            GetEntityManager().AddEntity(new SpotLightEntity(spotlight, m_LocalResources.GetModel("LightSphere"), transform));
        }

        GetCamera().SetPosition(v3(0.0, 2.0, 6.0));

        GetEntityManager().AddEntity(new Entity(
            m_LocalResources.GetModel("Plane"),
            TransformGroup::Build()
                .Add<Scale>(20.0)
                .Compose()
        ));

        GetEntityManager().AddEntity(new Entity(
            m_LocalResources.GetModel("Sphere"),
            TransformGroup::Build()
                .Add<Translate>(0.0, 1.5, 1.0)
                .Add<Scale>(1.0)
                .Compose()
        ));

        GetEntityManager().AddEntity(new Entity(
            m_LocalResources.GetModel("RedCube"),
            TransformGroup::Build()
                .Add<Translate>(1.0, 1.0, 6.0)
                .Add<Scale>(1.0)
                .Compose()
        ));

        GetEntityManager().AddEntity(new Entity(
            m_LocalResources.GetModel("GreenCube"),
            TransformGroup::Build()
                .Add<Translate>(-6.0, 3.0, 0.0)
                .Add<Scale>(1.0)
                .Compose()
        ));

        GetEntityManager().AddEntity(new Entity(
            m_LocalResources.GetModel("BlueCube"),
            TransformGroup::Build()
                .Add<Translate>(-6.0, 5.0, 0.0)
                .Add<Scale>(1.0)
                .Compose()
        ));

        GetEntityManager().AddEntity(new Entity(
            m_LocalResources.GetModel("BlueCube"),
            TransformGroup::Build()
                .Add<Translate>(5.0, 1.0, 1.0)
                .Add<Scale>(1.0)
                .Compose()
        ));

        GetEntityManager().AddEntity(new Entity(
            m_LocalResources.GetModel("GreenCube"),
            TransformGroup::Build()
                .Add<Translate>(-6.0, 3.0, 8.0)
                .Add<Scale>(1.0)
                .Compose()
        ));

        GetEntityManager().AddEntity(new Entity(
            m_LocalResources.GetModel("BlueCube"),
            TransformGroup::Build()
                .Add<Translate>(-6.0, 5.0, 6.0)
                .Add<Scale>(1.0)
                .Compose()
        ));

        GetEntityManager().AddEntity(new Entity(
            m_LocalResources.GetModel("RedCube"),
            TransformGroup::Build()
                .Add<Translate>(-6.0, 1.0, 6.0)
                .Add<Scale>(1.0)
                .Compose()
        ));

    }

    void ShadowScene::OnEvent(ZPG::Event& event) {
        Scene::OnEvent(event);

        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent& event) {
            if (event.GetKeyCode() == ZPG_KEY_T) {
                f32 size = 20.0f;
                GetCamera().SetOrthoProjection(-size, size, -size, size);
            }

            return false;
        });
    }

}
