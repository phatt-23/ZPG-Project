#include "Scene.h"
#include "../assets/models/nemec/sphere.h"

namespace RevolverScene {

RevolverScene::RevolverScene()
: m_LocalRes()
{
    ref<SpotLight> light = MakeRef<SpotLight>(
        ColorComponent(v4(1.0)),
        PositionComponent(v3(0.0)),
        DirectionComponent(v3(0.0, 0.0, -1.0)),
        BeamShapeComponent(20.0, 0.5),
        AttenComponent(0.0001, 0.0001, 0.1)
    );

    GetLightManager().AddLight(light);

    m_CameraController = MakeRef<FlashlightCameraController>(light, GetCamera());

    SetCameraController(m_CameraController);
}

void RevolverScene::OnLazyAttach() 
{
    m_LocalRes.LoadModel("ColtPython", "./assets/models/vendor/gameready_colt_python_revolver/scene.gltf");
    m_LocalRes.LoadModel("KorthRevolver", "./assets/models/vendor/korth_nxr_revolver/scene.gltf");
    m_LocalRes.LoadModel("Plane", "./assets/models/plane/plane.gltf");

    int gridSize = 3;
    float dist = 4;

    GetEntityManager().AddEntity(new Entity(
        m_LocalRes.GetModel("Plane"), 
        TransformGroup::Build().Add<Scale>(10.0f).Compose()));

    for (int i = -gridSize; i < gridSize; i++) {
        for (int j = -gridSize; j < gridSize; j++) {
            auto transform = TransformGroup::Build()
                .Add<Scale>(0.1)
                .Add<DynRotate>(0.0, 20.0)
                .Add<Translate>(dist*i, 0, dist*j)
                .Compose();

            GetEntityManager().AddEntity(new Entity(
                    m_LocalRes.GetModel("KorthRevolver"), 
                    transform));
        }
    }

    for (int i = -gridSize; i < gridSize; i++) {
        for (int j = -gridSize; j < gridSize; j++) {
            auto transform = TransformGroup::Build()
                .Add<Scale>(10)
                .Add<DynRotate>(0.0, 20.0)
                // .Add<Rotate>(90.0, v3(1.0, 0.0, 0.0))
                .Add<Translate>(dist*i, 0.0, dist*j)
                .Compose();

            GetEntityManager().AddEntity(
                new Entity(m_LocalRes.GetModel("ColtPython"), transform));
        }
    }


    firefly_material = MakeRef(new Material());
    firefly_material->SetEmissive(v4(firefly_color));

    auto firefly_model = Model::Create({
        Mesh::Create(VertexArray::Create({
                VertexBuffer::Create(nemec::sphere, sizeof(nemec::sphere), 
                                     {{ShaderDataType::Float3}, {ShaderDataType::Float3}})
            }),
            firefly_material)
    });

    m_LocalRes.AddModel("Firefly", firefly_model);

    for (int i = -gridSize; i < gridSize; i++) {
        auto firefly_light = MakeRef(new PointLight(v4(1.0), v3(0.0)));

        GetLightManager().AddLight(firefly_light);

        firefly_lights.push_back(firefly_light);

        GetEntityManager().AddEntity(new PointLightEntity(
            firefly_light, 
            firefly_model,
            TransformGroup::Build()
                .Add<Scale>(0.1)
                .Add<Translate>(dist*i, 1.5, dist*i)
                .Compose()));
    }

    ambient_light = new AmbientLight(v4(1.0, 1.0, 1.0, 0.001));
    GetLightManager().AddLight(ambient_light);

    dir_light = new DirectionalLight(v4(1.0, 1.0, 1.0, 0.8), v3(-1.0));
    GetLightManager().AddLight(dir_light);
}

} 
