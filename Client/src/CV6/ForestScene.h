#pragma once

#include "imgui.h"
#include "ZPGine.h"
#include "../../../assets/models/nemec/plain.h"
#include "../../../assets/models/nemec/tree.h"
#include "../../../assets/models/nemec/bushes.h"
#include "../../../assets/models/nemec/sphere.h"

namespace CV6 {

using namespace ZPG;

class ForestScene final : public ZPG::Scene {
public:
    ForestScene() : m_CameraController(GetCamera()) {
    }

    ResourceManager m_LocalResourceManager;

    void LoadMaterials() {
        m_ShaderProgram = m_ResourceManager->GetShaderProgram(CommonResources::SHADER_PROGRAM_DEFAULT_LIT);

        auto treeMaterial = MakeRef(new Material());
        treeMaterial->SetAlbedo(v4(0.0, 1.0, 0.0, 1.0));
        treeMaterial->SetShaderProgram(m_ShaderProgram);

        auto bushMaterial = MakeRef(new Material());
        bushMaterial->SetAlbedo(v4(0.0, 1.0, 0.0, 0.8));
        bushMaterial->SetShaderProgram(m_ShaderProgram);

        auto groundMaterial = MakeRef(new Material());
        groundMaterial->SetAlbedo(v4(0.8, 0.5, 0.0, 1.0));
        groundMaterial->SetShaderProgram(m_ShaderProgram);

        auto fireflyMaterial = MakeRef(new Material());
        fireflyMaterial->SetAlbedo(v4(1.0, 1.0, 0.4, 1.0));
        fireflyMaterial->SetShaderProgram(m_ResourceManager->GetShaderProgram(CommonResources::SHADER_PROGRAM_CONSTANT));

        m_LocalResourceManager.AddMaterial("Tree", treeMaterial);
        m_LocalResourceManager.AddMaterial("Bush", bushMaterial);
        m_LocalResourceManager.AddMaterial("Ground", groundMaterial);
        m_LocalResourceManager.AddMaterial("Firefly", fireflyMaterial);
    }

    void LoadModels() {
        BufferLayout nemecLayout = {
            {ShaderDataType::Float3, "a_Pos"},
            {ShaderDataType::Float3, "a_Normal"},
        };

        auto treeModel = Model::Create({
            Mesh::Create(
                VertexArray::Create({
                    VertexBuffer::Create(nemec::tree, sizeof(nemec::tree), nemecLayout)
                }),
                m_LocalResourceManager.GetMaterial("Tree")
            )
        });

        auto bushModel = Model::Create({
            Mesh::Create(
                VertexArray::Create({
                    VertexBuffer::Create(nemec::bushes, sizeof(nemec::bushes), nemecLayout)
                }),
                m_LocalResourceManager.GetMaterial("Bush")
            )
        });

        auto fireflyModel = Model::Create({
            Mesh::Create(
                VertexArray::Create({
                    VertexBuffer::Create(nemec::sphere, sizeof(nemec::sphere), nemecLayout)
                }),
                m_LocalResourceManager.GetMaterial("Firefly")
            )
        });


        auto groundModel = Model::Create({
            Mesh::Create(
                VertexArray::Create({
                    VertexBuffer::Create(nemec::plain, sizeof(nemec::plain), nemecLayout)
                }),
                m_LocalResourceManager.GetMaterial("Ground")
            )
        });

        m_LocalResourceManager.AddModel("Firefly", fireflyModel);
        m_LocalResourceManager.AddModel("Ground", groundModel);
        m_LocalResourceManager.AddModel("Tree", treeModel);
        m_LocalResourceManager.AddModel("Bush", bushModel);
    }

    void OnAttach() override {
        LoadMaterials();
        LoadModels();

        f32 groundSize = 40.0;

        // trees
        for (int i = 0; i < 100; i++) {
            auto transform = TransformGroup::Build()
                .Add<Translate>(
                    groundSize * Utility::GetRandomFloat(),
                    0.0,
                    groundSize * Utility::GetRandomFloat())
                .Add<Rotate>(180.0 * Utility::GetRandomFloat(), v3(0.0, 1.0, 0.0))
                .Compose();

            GetEntityManager().AddEntity(new Entity(m_LocalResourceManager.GetModel("Tree"), transform));
        }

        // bushes
        for (int i = 0; i < 100; i++) {
            auto transform = TransformGroup::Build()
                .Add<Translate>(
                    groundSize * Utility::GetRandomFloat(),
                    0.0,
                    groundSize * Utility::GetRandomFloat())
                .Add<Rotate>(180.0 * Utility::GetRandomFloat(), v3(0.0, 1.0, 0.0))
                .Compose();

            GetEntityManager().AddEntity(new Entity(
                m_LocalResourceManager.GetModel("Bush"),
                transform));
        }

        auto groundTransform = TransformGroup::Build()
            .Add<Scale>(groundSize)
            .Compose();

        // fireflies
        for (int i = 0; i < 20; i++) {
            auto pointLight = MakeRef<PointLight>(
                v4(1.0, 1.0, 1.0, 0.2),
                v3(0.0, 1.0, 0.0));

            GetLightManager().AddLight(pointLight);

            auto transform = TransformGroup::Build()
                .Add<Scale>(0.05f)
                .Add<Translate>(
                    Utility::GetRandomFloat() * 5.0f,
                    Utility::GetRandomFloat(0.0, 2.0),
                    Utility::GetRandomFloat() * 5.0f)
                .Add<DynRotate>(0.0f, 20.0f, v3(
                    Utility::GetRandomFloat(0.0, 1.0),
                    5.0,
                    Utility::GetRandomFloat(0.0, 1.0)))
                .Add<Translate>(
                    groundSize * Utility::GetRandomFloat(),
                    0.0,
                    groundSize * Utility::GetRandomFloat())
                .Compose();

            auto lightEntity = new PointLightEntity(
                pointLight,
                m_LocalResourceManager.GetModel("Firefly"),
                transform);

            GetEntityManager().AddEntity(lightEntity);
        }

        // ground
        GetEntityManager().AddEntity(new Entity(
            m_LocalResourceManager.GetModel("Ground"),
            groundTransform));

        // lights
        m_DirLight = new DirectionalLight(v4(0.1, 0.1, 1.0, 0.8), v3(-1.0));
        AddLight(m_DirLight);

        m_AmbientLight = new AmbientLight(v4(1.0, 1.0, 1.0, 0.001));
        AddLight(m_AmbientLight);
    }

    DirectionalLight* m_DirLight;
    AmbientLight* m_AmbientLight;

    void OnUpdate(Timestep& ts) override {
        Scene::OnUpdate(ts);
        m_CameraController.OnUpdate(ts);
        for (auto& entity : GetEntityManager().GetEntities()) {
            entity->Update(ts);
        }
    }

    void OnEvent(Event &event) override {
        Scene::OnEvent(event);
        m_CameraController.OnEvent(event);
    }

    void SetShaderProgram(const std::string& shaderProgramResource) {
        m_ShaderProgram = m_ResourceManager->GetShaderProgram(shaderProgramResource);
        m_LocalResourceManager.GetMaterial("Tree")->SetShaderProgram(m_ShaderProgram);
        m_LocalResourceManager.GetMaterial("Bush")->SetShaderProgram(m_ShaderProgram);
        m_LocalResourceManager.GetMaterial("Ground")->SetShaderProgram(m_ShaderProgram);
        // m_LocalResourceManager.GetMaterial("Firefly")->SetShaderProgram(m_ShaderProgram);
    }

    void OnImGuiRender() override {
        ImGui::Begin("Shader");
        if (ImGui::Button("Constant")) {
            SetShaderProgram(CommonResources::SHADER_PROGRAM_CONSTANT);
        }
        if (ImGui::Button("Lambert")) {
            SetShaderProgram(CommonResources::SHADER_PROGRAM_LAMBERT);
        }
        if (ImGui::Button("Phong")) {
            SetShaderProgram(CommonResources::SHADER_PROGRAM_PHONG);
        }
        if (ImGui::Button("Blinn-Phong")) {
            SetShaderProgram(CommonResources::SHADER_PROGRAM_BLINN_PHONG);
        }
        if (ImGui::Button("PBR")) {
            SetShaderProgram(CommonResources::SHADER_PROGRAM_PBR);
        }
        ImGui::End();

        ImGui::Begin("Lights");
        static v4 dirLightColor = m_DirLight->m_Color.GetColor();
        if (ImGui::SliderFloat4("DirLight Color", glm::value_ptr(dirLightColor), 0.0, 1.0)) {
            m_DirLight->m_Color.SetColor(dirLightColor);
        }
        static v4 ambientLightColor = m_AmbientLight->m_Color.GetColor();
        if (ImGui::SliderFloat4("AmbientLight Color", glm::value_ptr(ambientLightColor), 0.0, 1.0)) {
            m_AmbientLight->m_Color.SetColor(ambientLightColor);
        }

        ImGui::End();
    }

    ref<ShaderProgram> m_ShaderProgram;
    CameraController m_CameraController;
};

}
