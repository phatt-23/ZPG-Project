#pragma once

#include "ZPGine.h"

namespace Obhajoba
{
    using namespace ZPG;

    class NormalMapShowcaseScene : public Scene
    {
    public:
        NormalMapShowcaseScene()
        {
            ref<SpotLight> spotlight = MakeRef(new SpotLight(
                ColorComponent(v4(1.0)),
                PositionComponent(v3(0.0)),
                DirectionComponent(v3(0.0, 0.0, -1.0)),
                BeamShapeComponent(20.0, 0.5),
                AttenComponent(2.0f, v3(0.001, 0.0, 0.0))
            ));

            GetLightManager().AddLight(spotlight);

            SetCameraController(
                MakeRef<FlashlightCameraController>(spotlight, GetCamera()));
        }

        void OnAttach() override
        {
            m_Resource.LoadModel("Gun", 
                "./assets/models/vendor/korth_nxr_revolver/scene.gltf");
            m_Resource.LoadModel("NemecSphere", 
                "./assets/models/nemec-assets/normalmap/Sphere/model.obj");
            m_Resource.LoadModel("NemecBox", 
                "./assets/models/nemec-assets/normalmap/Box/model.obj");
            m_Resource.LoadModel("Plane", 
                "./assets/models/plane/plane.gltf");

            GetLightManager().AddLight(new AmbientLight(v4(0.1)));
            GetLightManager().AddLight(new DirectionalLight(
                v4(1.0, 1.0, 1.0, 0.8), v3(-0.06,-0.978,-0.201)));

            SetSky(Skybox::Create(SkyboxSpecification{ 
                .Directory = "./assets/textures/basic-skybox/" }));

            GetCamera().SetPosition(v3(0,2,30));

            // plane
            auto plane = new Entity(
                m_Resource.GetModel("Plane"),
                TransformGroup::Build()
                    .Add<Scale>(100.0)
                    .Compose()
            );


            auto revolver = new Entity(m_Resource.GetModel("Gun"), 
                       TransformGroup::Build() 
                       .Add<Scale>(1.0f)
                       .Add<Translate>(0,10,0)
                       .Compose());

            auto sphere = new Entity(m_Resource.GetModel("NemecSphere"), 
                                     TransformGroup::Build()
                                     .Add<Scale>(15.f)
                                     .Add<Translate>(25, 24, 14)
                                     .Compose());

            auto box = new Entity(m_Resource.GetModel("NemecBox"), 
                                     TransformGroup::Build()
                                     .Add<Scale>(15.f)
                                     .Add<Translate>(-20, 10, 20)
                                     .Compose());


            GetEntityManager().AddEntity(revolver);
            GetEntityManager().AddEntity(plane);
            GetEntityManager().AddEntity(sphere);
            GetEntityManager().AddEntity(box);
        }
    private:
        ResourceManager m_Resource;
    };
}
