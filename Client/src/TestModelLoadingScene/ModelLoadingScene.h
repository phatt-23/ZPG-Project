#pragma once

#include "ZPGine.h"

namespace TestModelLoadingNS {

class ModelLayer : public ZPG::Layer {
public:
    ModelLayer(ZPG::ResourceManager& resourceManager) : Layer(resourceManager) {
    }

    void OnAttach() override {
        using namespace ZPG;

        auto transform = CompoundTransform::Create();
        transform->Emplace<ScaleTransform>(0.1f * glm::vec3(1.0f));
        transform->Emplace<TranslationTransform>(glm::vec3(0.0, 0.0, 0.0));

        auto horseTransform = CompoundTransform::Create();
        horseTransform->Emplace<ScaleTransform>(0.1f * glm::vec3(1.0f));
        horseTransform->Emplace<TranslationTransform>(glm::vec3(0.0, 0.0, 0.0));

        m_Entities.push_back( CreateScope(new Entity(m_ResourceManager.GetModel("hyena"), transform)) );
        m_Entities.push_back( CreateScope(new Entity(m_ResourceManager.GetModel("horse"), horseTransform)) );
    }

    void OnUpdate(ZPG::SceneContext& context) override {
        for (auto& entity : m_Entities) {
            entity->Update(context.m_Timestep);
        }
    }

    void OnRender(const ZPG::RenderContext& context) override {
        using namespace ZPG;

        Renderer::BeginDraw(context.m_Camera);
        for (auto& entity : m_Entities) {
            Renderer::Submit(*m_ResourceManager.GetShaderProgram("basic_pos_normal_uv"), *entity);
        }
        Renderer::EndDraw();
    }
private:
    std::vector<ZPG::Scope<ZPG::Entity>> m_Entities;
};


class ModelLoadingScene : public ZPG::Scene {
public:
    ModelLoadingScene() : m_CameraController(GetCamera()) {
    }

    void OnAttach() override {
        // The resource manager can be 'overriden' by the client if they should choose to use their 
        // own instead of the global resource manager.
        m_LocalResourceManager = ZPG::CreateScope<ZPG::ResourceManager>();
        m_LocalResourceManager->LoadModel("hyena", "assets/models/hyena/hyena_demo_free_download/scene.gltf");
        m_LocalResourceManager->LoadModel("horse", "assets/models/er_l/scene.gltf");
        m_LocalResourceManager->LoadShaderProgram("basic_pos_normal_uv", 
            "./assets/shaders/vertex/basic_pos_normal_uv.vert", 
            "./assets/shaders/fragment/normal_as_color.frag");

        PushLayer(new ModelLayer(*m_LocalResourceManager));
    }

    void OnUpdate([[maybe_unused]] ZPG::Timestep ts) override {
        m_CameraController.OnUpdate(ts);
        Scene::OnUpdate(ts);
    }

    void OnEvent([[maybe_unused]] ZPG::Event& event) override {
        m_CameraController.OnEvent(event);
        Scene::OnEvent(event);
    }
private:
    ZPG::CameraController m_CameraController;
    ZPG::Scope<ZPG::ResourceManager> m_LocalResourceManager;
};

}
