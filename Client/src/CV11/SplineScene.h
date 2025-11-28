//
// Created by phatt on 11/28/25.
//
#pragma once

#include "ZPGine.h"

namespace CV11
{
    using namespace ZPG;

    class SplineScene : public Scene
    {
    public:
        void OnAttach() override
        {
            m_Res.LoadModel("Shrek", "./assets/models/vendor/shrek/shrek.obj");
            m_Res.LoadModel("Fiona", "./assets/models/vendor/shrek/fiona.obj");

            GetLightManager().AddLight(new AmbientLight(v4(0.1)));
            GetLightManager().AddLight(new DirectionalLight(v4(1.0, 1.0, 1.0, 0.8), v3(-1,-1,-1)));

            SetSky(Skybox::Create(SkyboxSpecification{ .Directory = "./assets/textures/basic-skybox/" }));

            GetCamera().SetPosition(v3(0.0, 2.0, 0.0));

            GetEntityManager().AddEntity(new Entity(
                GetResourceManager().GetModel(CommonResources::MODEL_PLANE),
                TransformGroup::Build()
                    .Add<Scale>(60.0)
                    .Compose()
            ));

            GetEntityManager().AddEntity(new Entity(
                m_Res.GetModel("Shrek"),
                TransformGroup::Build()
                    .Add<Scale>(2.0)
                    .Add<SplineMovement>(
                        v3{-20, 0, 20},
                        vec<SplineSegment>{
                            {
                                {-20, 0.0, -20},
                                {20, 0.0, -20},
                                {20, 0.0, 20},
                            },
                            {
                                {40, 20.0, 20},
                                {40, 20.0, -20},
                                {20, 20.0, -40},
                            },
                            {
                                {20, 40.0, -20},
                                {20, 60.0, 20},
                                {-20, 0.0, 20},
                            },
                        },
                        8.0f,
                        MovementMode::PingPong)
                    .Compose()
            ));

            GetEntityManager().AddEntity(new Entity(
                m_Res.GetModel("Fiona"),
                TransformGroup::Build()
                    .Add<Scale>(2.0)
                    .Add<SplineMovement>(
                        v3{-20, 0, 20},
                        vec<SplineSegment>{
                            {
                                {-20, 0.0, -20},
                                {20, 0.0, -20},
                                {20, 0.0, 20},
                            },
                            {
                                {40, 0.0, 20},
                                {40, 0.0, -20},
                                {20, 0.0, -40},
                            },
                            {
                                {20, 0.0, -20},
                                {20, 0.0, 20},
                                {-20, 0.0, 20},
                            },
                        },
                        8.0f,
                        MovementMode::PingPong)
                    .Compose()
            ));
        }
    private:
        ResourceManager m_Res;
    };

}
