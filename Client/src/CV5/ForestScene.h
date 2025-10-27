#pragma once

#include "imgui.h"
#include "ZPGine.h"
#include "../../../assets/models/nemec/plain.h"
#include "../../../assets/models/nemec/tree.h"
#include "../../../assets/models/nemec/bushes.h"

namespace CV5 {

using namespace ZPG;

class ForestScene final : public ZPG::Scene {
public:
    ForestScene() : m_CameraController(GetCamera()) {
        m_TreeMaterial = MakeRef(new Material());
        m_BushMaterial = MakeRef(new Material());
        m_GroundMaterial = MakeRef(new Material());
    }

    ref<Material> m_GroundMaterial;
    ref<Material> m_TreeMaterial;
    ref<Material> m_BushMaterial;

    void OnAttach() override {
        m_ShaderProgram = GetResourceManager().GetShaderProgram(CommonResources::SHADER_PROGRAM_DEFAULT);

        m_TreeMaterial->SetAlbedo(v4(0.0, 1.0, 0.0, 1.0));
        m_TreeMaterial->SetShaderProgram(m_ShaderProgram);

        m_BushMaterial->SetAlbedo(v4(0.0, 1.0, 0.0, 0.8));
        m_BushMaterial->SetShaderProgram(m_ShaderProgram);

        auto treeModel = Model::Create({
            Mesh::Create(
                VertexArray::Create({
                    VertexBuffer::Create(nemec::tree, sizeof(nemec::tree), {
                        {ShaderDataType::Float3, "a_Pos"},
                        {ShaderDataType::Float3, "a_Normal"},
                    })
                }),
                m_TreeMaterial
            )
        });

        auto bushModel = Model::Create({
            Mesh::Create(
                VertexArray::Create({
                    VertexBuffer::Create(nemec::bushes, sizeof(nemec::bushes), {
                        {ShaderDataType::Float3, "a_Pos"},
                        {ShaderDataType::Float3, "a_Normal"},
                    })
                }),
                m_TreeMaterial
            )
        });

        m_GroundMaterial->SetAlbedo(v4(0.4, 0.2, 0.0, 1.0));
        m_GroundMaterial->SetShaderProgram(m_ShaderProgram);

        auto groundModel = Model::Create({
            Mesh::Create(
                VertexArray::Create({
                    VertexBuffer::Create(nemec::plain, sizeof(nemec::plain), {
                        {ShaderDataType::Float3, "a_Pos"},
                        {ShaderDataType::Float3, "a_Normal"},
                    })
                }),
                m_GroundMaterial
            )
        });


        f32 groundSize = 20.0;

        for (int i = 0; i < 100; i++) {
            auto transform = TransformGroup::Build()
                .Add<Translate>(
                    groundSize * Utility::GetRandomFloat(),
                    0.0,
                    groundSize * Utility::GetRandomFloat())
                .Add<Rotate>(180.0 * Utility::GetRandomFloat(), v3(0.0, 1.0, 0.0))
                .Compose();

            GetEntityManager().AddEntity(new Entity(treeModel, transform));
        }

        for (int i = 0; i < 0; i++) {
            auto transform = TransformGroup::Build()
                .Add<Translate>(
                    groundSize * Utility::GetRandomFloat(),
                    0.0,
                    groundSize * Utility::GetRandomFloat())
                .Add<Rotate>(180.0 * Utility::GetRandomFloat(), v3(0.0, 1.0, 0.0))
                .Compose();

            GetEntityManager().AddEntity(new Entity(bushModel, transform));
        }

        auto groundTransform = TransformGroup::Build()
            .Add<Scale>(groundSize)
            .Compose();

        GetEntityManager().AddEntity(new Entity(groundModel, groundTransform));

        AddLight(new DirectionalLight(v4(1.0, 1.0, 1.0, 0.2), v3(-1.0)));
        AddLight(new AmbientLight(v4(1.0, 1.0, 1.0, 0.2)));
        AddLight(new PointLight(v4(1.0), v3(0.0, 1.0, 0.0)));
        AddLight(new PointLight(v4(1.0), v3(0.0, 1.0, 0.0)));
        AddLight(new PointLight(v4(1.0), v3(0.0, 1.0, 0.0)));
        AddLight(new PointLight(v4(1.0), v3(0.0, 1.0, 0.0)));
        AddLight(new PointLight(v4(1.0), v3(0.0, 1.0, 0.0)));
    }

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
        m_ShaderProgram = GetResourceManager().GetShaderProgram(shaderProgramResource);
        m_GroundMaterial->SetShaderProgram(m_ShaderProgram);
        m_TreeMaterial->SetShaderProgram(m_ShaderProgram);
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
    }

    ref<ShaderProgram> m_ShaderProgram;
    CameraController m_CameraController;
};

}
