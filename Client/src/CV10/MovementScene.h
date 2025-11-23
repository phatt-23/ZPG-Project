#include "Transform/MovementTransform/BezierMovement.h"
#include "Transform/MovementTransform/CircleMovement.h"
#include "Transform/MovementTransform/LineMovement.h"
#include "ZPGine.h"

namespace CV10
{
    using namespace ZPG;

    class MovementScene : public Scene
    {
    public:
        void OnAttach() override
        {
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
                GetResourceManager().GetModel(CommonResources::MODEL_CUBE),
                TransformGroup::Build()
                    .Add<Scale>(1.0)
                    .Add<LineMovement>( 
                        v3(0.0, 2.0, 0.0), 
                        v3(4.0, 4.0, -4.0), 
                        2.f,
                        MovementMode::Repeat)
                    .Compose()
            ));

            GetEntityManager().AddEntity(new Entity(
                GetResourceManager().GetModel(CommonResources::MODEL_SPHERE),
                TransformGroup::Build()
                    .Add<Scale>(1.0)
                    .Add<LineMovement>(
                        v3(-2.0, 2.0, 0.0), 
                        v3(-2.0, 8.0, 0.0), 2, 
                        MovementMode::Repeat)
                    .Compose()
            ));

            GetEntityManager().AddEntity(new Entity(
                GetResourceManager().GetModel(CommonResources::MODEL_SPHERE),
                TransformGroup::Build()
                    .Add<Scale>(1.0)
                    .Add<CircleMovement>(
                        v3(0.0, 6.0, 6.0), 
                        5.f, 
                        v3(0.0, 1.0, 0.0), 
                        4.f,
                        MovementMode::Repeat)
                    .Compose()
            ));

            GetEntityManager().AddEntity(new Entity(
                GetResourceManager().GetModel(CommonResources::MODEL_CUBE),
                TransformGroup::Build()
                    .Add<Scale>(1.0)
                    .Add<DynRotate>(0.0, 90.0, v3(1.0, 1.0, 0.0))
                    .Add<BezierMovement>(vec<v3>{
                        v3(20.0, 2.0, -20.0),
                        v3(0.0, 20.0, 20.0),
                        v3(-20.0, -20.0, -20.0),
                        v3(0.0, 40.0, -60.0),
                        v3(20.0, 2.0, -20.0),
                    }, 4.0f, MovementMode::Repeat)
                    .Compose()
            ));

            GetEntityManager().AddEntity(new Entity(
                GetResourceManager().GetModel(CommonResources::MODEL_SPHERE),
                TransformGroup::Build()
                    .Add<Scale>(1.0)
                    .Add<DynRotate>(0.0, 90.0, v3(1.0, 1.0, 0.0))
                    .Add<PolyLineMovement>(vec<v3>{
                        v3(20.0, 2.0, -20.0),
                        v3(0.0, 20.0, 20.0),
                        v3(-20.0, -20.0, -20.0),
                        v3(0.0, 40.0, -60.0),
                        v3(20.0, 2.0, -20.0),
                    }, 4.0f, MovementMode::Repeat)
                    .Compose()
            ));
        }
    };
}
