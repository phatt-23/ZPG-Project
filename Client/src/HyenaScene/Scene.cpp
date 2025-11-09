#include "Scene.h"
#include "../assets/models/nemec/sphere.h"

namespace HyenaScene {

HyenaScene::HyenaScene()
{

}

void HyenaScene::OnAttach() {
    m_LocalRes.LoadModel("Hyena", "./assets/models/hyena/hyena_demo_free_download/scene.gltf");

    auto spotlight = MakeRef(new SpotLight(v4(1.0), v3(0.0), v3(1.0), 20.f, 0.5f, AttenComponent(0.001, 0.001, 0.1f)));
    auto controller = MakeRef(new FlashlightCameraController(spotlight, GetCamera()));
    SetCameraController(controller);

    int grid_size = 10;

    for (int i = -grid_size; i < grid_size; i++) {
        for (int j = -grid_size; j < grid_size; j++) {
            auto transform = TransformGroup::Build()
                .Add<Scale>(0.005)
                .Add<Rotate>(180.0, v3(1.0, 0.0, 0.0))
                .Add<DynRotate>(0.0, 20.0, v3(0.0, 1.0, 0.0))
                .Add<Translate>(i, 0.0, j)
                .Compose();

            GetEntityManager().AddEntity(
                new Entity(m_LocalRes.GetModel("Hyena"), transform));
        }
    }


    fireflyMaterial = MakeRef(new Material());
    fireflyMaterial->SetEmissive(v4(fireflyColor));

    auto firefly_model = Model::Create({
        Mesh::Create(VertexArray::Create({
                VertexBuffer::Create(nemec::sphere, sizeof(nemec::sphere), 
                                     {{ShaderDataType::Float3}, {ShaderDataType::Float3}})
            }),
            fireflyMaterial)
    });

    m_LocalRes.AddModel("Firefly", firefly_model);

    for (int i = -grid_size; i < grid_size; i++) {
        auto firefly_light = MakeRef(new PointLight(v4(1.0), v3(0.0), AttenComponent(3.0, 4.0, 5.0f)));
        GetLightManager().AddLight(firefly_light);
        fireflyLights.push_back(firefly_light);

        PointLightEntity *pointLightEntity = new PointLightEntity(
            firefly_light, firefly_model,
            TransformGroup::Build().Add<Scale>(0.1).Add<Translate>(i, 1.0, i).Compose());

        GetEntityManager().AddEntity(pointLightEntity);
    }
}

void HyenaScene::OnImGuiRender() {
    Scene::OnImGuiRender();

    ImGui::Begin("Shader program");

    static const char* shader_programs[] = {
        CommonResources::SHADER_PROGRAM_CONSTANT,
        CommonResources::SHADER_PROGRAM_LAMBERT,
        CommonResources::SHADER_PROGRAM_PHONG,
        CommonResources::SHADER_PROGRAM_BLINN_PHONG,
        CommonResources::SHADER_PROGRAM_PBR,
    };

    for (int i = 0; i < ZPG_ARRAY_LENGTH(shader_programs); i++) {
        if (ImGui::RadioButton(shader_programs[i], m_CurrentShaderProgramRoute == shader_programs[i])) {
            m_CurrentShaderProgramRoute = (char*)shader_programs[i];

            auto shaderProgram = GetResourceManager().GetShaderProgram(m_CurrentShaderProgramRoute);

            for (auto& [_, model] : m_LocalRes.GetModels()) {
                for (auto& mesh : model->GetMeshes()) {
                    mesh->GetMaterial()->SetShaderProgram(shaderProgram);
                }
            }
        }
    }

    if (ImGui::SliderFloat4("firefly color", glm::value_ptr(fireflyColor), 0.0, 1.0)) {
        fireflyMaterial->SetEmissive(fireflyColor);
        for (ref<PointLight> light : fireflyLights)
            light->Color.Set(fireflyColor);
    }

    ImGui::End();
}

} 
