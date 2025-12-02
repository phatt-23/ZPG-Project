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
            // m_Resource.LoadModel("NemecSphere", 
            //  "./assets/models/nemec-assets/normalmap/Sphere/model.obj");

            GetLightManager().AddLight(new AmbientLight(v4(0.1)));
            GetLightManager().AddLight(new DirectionalLight(
                v4(1.0, 1.0, 1.0, 0.8), v3(-1,-1,-1)));

            SetSky(Skybox::Create(SkyboxSpecification{ 
                .Directory = "./assets/textures/basic-skybox/" }));


            // plane
            auto plane = new Entity(
                GetResourceManager().GetModel(CommonResources::MODEL_PLANE),
                TransformGroup::Build()
                    .Add<Scale>(100.0)
                    .Compose()
            );


            auto revolver = new Entity(m_Resource.GetModel("Gun"), 
                       TransformGroup::Build() 
                       .Add<Scale>(1.0f)
                       .Add<Translate>(0,10,0)
                       .Compose());


            GetEntityManager().AddEntity(revolver);
            GetEntityManager().AddEntity(plane);
        }
    private:
        ResourceManager m_Resource;
    };
}
