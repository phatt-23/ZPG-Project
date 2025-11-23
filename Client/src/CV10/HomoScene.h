#include "ZPGine.h"

namespace CV10
{
    using namespace ZPG;

    class HomoScene : public Scene
    {
        void OnAttach() override
        {
            GetLightManager().AddLight(new AmbientLight(v4(0.05)));
            GetLightManager().AddLight(new DirectionalLight(v4(1.0, 1.0, 1.0, 0.8), v3(-1,-1,-1)));

            m_Res.LoadModel("Plane", "./assets/models/plane/plane.gltf");
            m_Res.LoadModel("RedCube", "./assets/models/cube/cube.gltf");
            m_Res.LoadModel("Shrek", "./assets/models/vendor/shrek/shrek.obj");

            m_Res.GetModel("RedCube")->GetMeshes().front()->GetMaterial()->SetAlbedo(v4(1,0,0,1));
            m_Res.GetModel("Plane")->GetMeshes().front()->GetMaterial()->SetMetallic(0);
            m_Res.GetModel("RedCube")->GetMeshes().front()->GetMaterial()->SetMetallic(0);

            GetCamera().SetPosition(v3(0.0, 2.0, 6.0));

            GetEntityManager().AddEntity(new Entity(
                m_Res.GetModel("Plane"),
                TransformGroup::Build()
                    .Add<Scale>(20.0)
                    .Compose()
            ));

            GetEntityManager().AddEntity(new Entity(
                m_Res.GetModel("RedCube"),
                TransformGroup::Build()
                    .Add<Translate>(0.0, 1.0, 0.0)
                    .Add<Scale>(1.0)
                    .Compose()
            ));

            m4 transformMatrix(1.0);
            transformMatrix[3][3] = 20.f;  // objekt se zmensi 20x

            GetEntityManager().AddEntity(new Entity(
                m_Res.GetModel("RedCube"),
                TransformGroup::Build()
                    .Add<Scale>(1.0)
                    .Add<Transform>(transformMatrix)
                    .Add<Translate>(2.0, 1.0, 0.0)
                    .Compose()
            ));

            GetEntityManager().AddEntity(new Entity(
                m_Res.GetModel("Shrek"),
                TransformGroup::Build()
                    .Add<Scale>(1.0)
                    .Add<Translate>(4.0, 2.0, 0.0)
                    .Compose()
            ));

            GetEntityManager().AddEntity(new Entity(
                m_Res.GetModel("Shrek"),
                TransformGroup::Build()
                    .Add<Scale>(1.0)
                    .Add<Transform>(transformMatrix)
                    .Add<Translate>(6.0, 2.0, 0.0)
                    .Compose()
            ));
        }
    private:
        ResourceManager m_Res;
    };
}
