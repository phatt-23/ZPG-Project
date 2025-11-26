//
// Created by phatt on 11/26/25.
//
#pragma once

#include "ZPGine.h"

namespace CV10
{
    using namespace ZPG;

    class SphereScene : public Scene
    {
    public:
        void OnAttach() override {
            GetLightManager().AddLight(new AmbientLight(v4(0.1)));
            GetLightManager().AddLight(new PointLight(v4(1.0), v3(0.0)));

            GetEntityManager().AddEntity(new Entity(
                GetResourceManager().GetModel(CommonResources::MODEL_SPHERE),
                TransformGroup::Build()
                    .Add<Translate>(v3(1.0, 1.0, 0.0))
                    .Compose()
            ));
            GetEntityManager().AddEntity(new Entity(
                GetResourceManager().GetModel(CommonResources::MODEL_SPHERE),
                TransformGroup::Build()
                    .Add<Translate>(v3(-1.0, 1.0, 0.0))
                    .Compose()
            ));
            GetEntityManager().AddEntity(new Entity(
                GetResourceManager().GetModel(CommonResources::MODEL_SPHERE),
                TransformGroup::Build()
                    .Add<Translate>(v3(1.0, -1.0, 0.0))
                    .Compose()
            ));
            GetEntityManager().AddEntity(new Entity(
                GetResourceManager().GetModel(CommonResources::MODEL_SPHERE),
                TransformGroup::Build()
                    .Add<Translate>(v3(-1.0, -1.0, 0.0))
                    .Compose()
            ));
        }
    };


}