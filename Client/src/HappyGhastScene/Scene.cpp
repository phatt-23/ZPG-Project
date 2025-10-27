#include "Scene.h"
#include "../assets/models/nemec/sphere.h"

namespace HappyGhastScene {

HappyGhastScene::HappyGhastScene() : m_Controller(GetCamera()), m_LocalRes() {}

void HappyGhastScene::OnLazyAttach() {
    m_LocalRes.LoadModel("Seoul", "./assets/models/korth_nxr_revolver/scene.gltf");

    int grid_size = 2;
    float dist = 4;

    for (int i = -grid_size; i < grid_size; i++) {
        for (int j = -grid_size; j < grid_size; j++) {
            auto transform = TransformGroup::Build()
                .Add<Scale>(0.1)
                // .Add<Rotate>(90.0, v3(1.0, 0.0, 0.0))
                .Add<Translate>(dist*i, 0, dist*j)
                .Compose();

            GetEntityManager().AddEntity(
                new Entity(m_LocalRes.GetModel("Seoul"), transform));
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

    for (int i = -grid_size; i < grid_size; i++) {
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

void HappyGhastScene::OnUpdate(Timestep &ts) {
    Scene::OnUpdate(ts);
    m_Controller.OnUpdate(ts);
}

void HappyGhastScene::OnEvent(Event &event) {
    Scene::OnEvent(event);
    m_Controller.OnEvent(event);
}

void HappyGhastScene::OnImGuiRender() {
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

    if (ImGui::SliderFloat4("firefly color", glm::value_ptr(firefly_color), 0.0, 1.0)) {
        firefly_material->SetEmissive(firefly_color);
        for (ref<PointLight> light : firefly_lights)
            light->m_Color.SetColor(firefly_color);
    }

    static v4 dirLightColor = dir_light->m_Color.GetColor();
    if (ImGui::SliderFloat4("DirLight Color", glm::value_ptr(dirLightColor), 0.0, 1.0)) {
        dir_light->m_Color.SetColor(dirLightColor);
    }
    static v4 ambientLightColor = ambient_light->m_Color.GetColor();
    if (ImGui::SliderFloat4("AmbientLight Color", glm::value_ptr(ambientLightColor), 0.0, 1.0)) {
        ambient_light->m_Color.SetColor(ambientLightColor);
    }

    ImGui::End();
}

} 
