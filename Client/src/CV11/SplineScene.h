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
            m_Res.LoadModel("Formula", "./assets/models/nemec-assets/formula1.obj");

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

            GetEntityManager().AddEntity(new Entity(
                m_Res.GetModel("Shrek"),
                TransformGroup::Build()
                    .Add<Scale>(2.0)
                    .Add<SplineMovement>(
                        v3{-20, 0, 20},   // starting point
                        vec<SplineSegment>{
                            {
                                {-20, 0.0, -20},   // control point 1
                                {20, 0.0, -20},    // control point 2
                                {20, 0.0, 20},     // end point of the first segment
                            },
                            {
                                {40, 20.0, 20},     // there's only three control points here
                                {40, 20.0, -20},    // because the first control point is
                                {20, 20.0, -40},    // the last control point of the previous segment
                            },
                            {
                                {20, 40.0, -20},
                                {20, 60.0, 20},
                                {-20, 0.0, 20},  // final end point
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
                                {-20, 0, 20},      // The segments can also 
                                {-20, 0.0, -20},   // be fully defined by all four points.
                                {20, 0.0, -20},    // This way we could make a non-continuous spline,
                                {20, 0.0, 20},     // though it is something we probably want to avoid.
                            },
                            {
                                {20, 0.0, 20},
                                {40, 0.0, 20},
                                {40, 0.0, -20},
                                {20, 0.0, -40},
                            },
                            {
                                {20, 0.0, -40},
                                {20, 0.0, -20},
                                {20, 0.0, 20},
                                {-20, 0.0, 20},
                            },
                        },
                        8.0f,
                        MovementMode::PingPong)
                    .Compose()
            ));
        
            GetEntityManager().AddEntity(new Entity(
                m_Res.GetModel("Formula"),
                TransformGroup::Build()
                    .Add<Rotate>(90.0f, v3(0,1,0))
                    .Add<Scale>(0.25)
                    .Add<SplineMovement>(
                        v3{-20, 0, 20},                 // starting point (P0)
                        vec<SplineSegment>{
                            {
                                {-20, 0.0, -20},        // control point 1 (P1)
                                {20, 0.0, -20},         // control point 2 (P2)
                                {20, 0.0, 20},          // end point of this segment (P3)
                            },
                            {
                                {3*40, 0.0, -20},       // control point 2 (P2)
                                {20, 0.0, -40},         // end point for this segment (P3)
                                                        // There's only two control points here.
                                                        // The first is the last control point of the previous segment.
                                                        // The control point 1, is calculated from the previous segment's P2 and P3.
                            },
                            {
                                {20, 0.0, 20},          // control point 2 (P3)
                                {-20, 0.0, 20},         // final end point (P4)
                            },
                        },
                        8.0f,
                        MovementMode::Repeat)
                    .Compose()
            ));
        }
    private:
        ResourceManager m_Res;
    };

}
