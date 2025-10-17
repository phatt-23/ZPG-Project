#pragma once

#include "ZPGine.h"

namespace CV5 {

using namespace ZPG;

class SpheresScene : public Scene {
public:
    SpheresScene() : m_CameraController(GetCamera()) {
    }

    void OnAttach() override {
        // copied
        m_Model = MakeRef<Model>(*m_ResourceManager->GetModel(CommonResources::MODEL_SPHERE));

        std::array<v3, 4> positions({
            {-1.0, -1.0, 0.0},
            {1.0, -1.0, 0.0},
            {1.0, 1.0, 0.0},
            {-1.0, 1.0, 0.0},
        });

        for (v3 pos : positions) {
            auto transform = TransformGroup::Build()
                .Add<Scale>(2.0)
                .Add<Translate>(3.0f * pos)
                .Compose();

            GetEntityManager().AddEntity(new Entity(m_Model, transform));
        }

        AddLight(new AmbientLight(v4(1.0, 1.0, 1.0, 0.2)));
        AddLight(new PointLight(v4(1.0), v3(0.0)));

        GetCamera().SetPosition(v3(0.0, 0.0, 4.0));
    }

    void OnResume() override {
        Scene::OnResume();
        Renderer::SetLights(GetLightManager().GetLights());
    }

    void OnUpdate(Timestep ts) override {
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

    void OnRender(Timestep ts) override {
        Renderer::BeginDraw(GetCamera());
        Renderer::SetLights(GetLightManager().GetLights());
        for (auto& entity : GetEntityManager().GetEntities()) {
            Renderer::SumbitEntity(*entity);
        }
        Renderer::EndDraw();
    }

    void SetShaderProgram(const std::string& shaderProgramResource) {
        m_ShaderProgram = m_ResourceManager->GetShaderProgram(shaderProgramResource);

        for (auto& mesh : m_Model->GetMeshes())
            mesh->GetMaterial()->SetShaderProgram(m_ShaderProgram);
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
        ImGui::End();
    }

    ref<Model> m_Model;
    ref<ShaderProgram> m_ShaderProgram;
    CameraController m_CameraController;
};

}
