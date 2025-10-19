#pragma once

#include "imgui.h"
#include "ZPGine.h"

namespace CV5 {

using namespace ZPG;

class SolarSystemScene : public ZPG::Scene {
public:
    SolarSystemScene() : m_CameraController(GetCamera()) {
        m_ShaderProgram = m_ResourceManager->GetShaderProgram(CommonResources::SHADER_PROGRAM_DEFAULT_LIT);
    }

    ref<Material> m_SunMaterial;
    ref<Material> m_EarthMaterial;
    ref<Material> m_MoonMaterial;

    void OnAttach() override {
        auto sphereVAO = VertexArray::Create({
            VertexBuffer::Create(nemec::sphere, sizeof(nemec::sphere), {
                {ShaderDataType::Float3, "a_Pos"},
                {ShaderDataType::Float3, "a_Normal"},
            })
        });

        m_SunMaterial = MakeRef<Material>();
        m_SunMaterial->SetShaderProgram(m_ShaderProgram);
        m_SunMaterial->SetAlbedo(v4(1.0, 0.5, 0.0, 1.0));
        m_SunMaterial->SetEmissive(v4(1.0, 0.8, 0.0, 1.0));

        m_EarthMaterial = MakeRef<Material>();
        m_EarthMaterial->SetShaderProgram(m_ShaderProgram);
        m_EarthMaterial->SetAlbedo(v4(0.0, 0.0, 1.0, 1.0));

        m_MoonMaterial = MakeRef<Material>();
        m_MoonMaterial->SetShaderProgram(m_ShaderProgram);
        m_MoonMaterial->SetAlbedo(v4(0.5, 0.5, 0.5, 1.0));


        auto sunModel = Model::Create({ Mesh::Create(sphereVAO, m_SunMaterial) });
        auto earthModel = Model::Create({ Mesh::Create(sphereVAO, m_EarthMaterial) });
        auto moonModel = Model::Create({ Mesh::Create(sphereVAO, m_MoonMaterial) });

        auto sunWorld = TransformGroup::Build()
            .Add<DynRotate>(0.0f, 25, v3(0.0, 1.0, 0.0))
            .Compose();

        auto sun = TransformGroup::Build()
            .Add<Scale>(v3(0.4f))
            .Include(sunWorld)
            .Compose();

        auto earthWorld = TransformGroup::Build()
            .WithParent(sunWorld)
            .Add<Translate>(glm::cross(v3(1,1,0), v3(0,1,0)))
            .Add<DynRotate>(0.0f, 45, v3(1,1,0))
            .Compose();

        auto earth = TransformGroup::Build()
            .Add<Scale>(v3(0.2f))
            .Include(earthWorld)
            .Compose();

        auto moon = TransformGroup::Build()
            .WithParent(earthWorld)
            .Add<Scale>(0.1f)
            .Add<Translate>(0.4f * glm::normalize(glm::cross(v3(0,1,1), v3(0,1,0))))
            .Add<DynRotate>(0.0, 90, v3(0,1,1))
            .Compose();

        GetEntityManager().AddEntity(new Entity(sunModel, sun));
        GetEntityManager().AddEntity(new Entity(earthModel, earth));
        GetEntityManager().AddEntity(new Entity(moonModel, moon));

        AddLight(new AmbientLight(v4(1.0, 1.0, 1.0, 0.2)));
        AddLight(new PointLight(v4(1.0), v3(0.0)));

        // make this happen:
        // m_EntityRegistry->AddComponent<LightComponent>(sunEntity);

        GetCamera().SetPosition(v3(0.0, 0.0, 4.0));
    }

    void OnResume() override {
        Scene::OnResume();
        Renderer::SetLights(GetLightManager().GetLights());
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

    void OnRender(Timestep& ts) override {
        Renderer::BeginDraw(GetCamera());
        Renderer::SetLights(GetLightManager().GetLights());
        for (auto& entity : GetEntityManager().GetEntities()) {
            Renderer::SubmitEntity(*entity);
        }
        Renderer::EndDraw();
    }

    void SetShaderProgram(const std::string& shaderProgramResource) {
        m_ShaderProgram = m_ResourceManager->GetShaderProgram(shaderProgramResource);
        m_SunMaterial->SetShaderProgram(m_ShaderProgram);
        m_EarthMaterial->SetShaderProgram(m_ShaderProgram);
        m_MoonMaterial->SetShaderProgram(m_ShaderProgram);
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
