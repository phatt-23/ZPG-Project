#pragma once

#include "ZPGine.h"

namespace Obhajoba
{
    using namespace ZPG;

    class SphereScene : public Scene
    {
    public:
        void OnAttach() override 
        {
            m_Resource.LoadModel("Sphere", "./assets/models/sphere/sphere_smooth.gltf");

            AddLight(new AmbientLight(v4(0.1)));
            AddLight(new PointLight(v4(1.0), v3(0.0)));

            AddEntity(new Entity(
                m_Resource.GetModel("Sphere"),
                TransformGroup::Build()
                    .Add<Translate>(1.5f * v3(1.0, 1.0, 0.0))
                    .Compose()
            ));
            AddEntity(new Entity(
                m_Resource.GetModel("Sphere"),
                TransformGroup::Build()
                    .Add<Translate>(1.5f * v3(-1.0, 1.0, 0.0))
                    .Compose()
            ));
            AddEntity(new Entity(
                m_Resource.GetModel("Sphere"),
                TransformGroup::Build()
                    .Add<Translate>(1.5f * v3(1.0, -1.0, 0.0))
                    .Compose()
            ));
            AddEntity(new Entity(
                m_Resource.GetModel("Sphere"),
                TransformGroup::Build()
                    .Add<Translate>(1.5f * v3(-1.0, -1.0, 0.0))
                    .Compose()
            ));
        }
    private:
        ResourceManager m_Resource;
    };
}
