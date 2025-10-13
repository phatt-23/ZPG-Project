#pragma once

#include "ZPGine.h"
#include "Models/tree.h"
#include "Models/bushes.h"

namespace ForestSceneNS {


class TreesLayer : public ZPG::Layer {
public:
    TreesLayer() {
    }

    void OnAttach() override {
        using namespace ZPG;

        auto VBO = VertexBuffer::Create(tree, sizeof(tree), {
            {ShaderDataType::Float3, "a_Pos"},
            {ShaderDataType::Float3, "a_Normal"},
        });

        auto VAO = VertexArray::Create({ VBO });
        auto mesh = CreateRef<Mesh>(VAO);
        m_Model = CreateRef(new Model({ mesh }));

        for (int i = 0; i < 25; i++) {
            auto transform = CreateRef<Translate>(
                10.f * glm::vec3(Utility::GetRandomFloat(), 0.0f, Utility::GetRandomFloat())
            );

            m_Entities.push_back(CreateScope(new Entity(m_Model, transform)));
        }
    }

    void OnUpdate(ZPG::SceneContext& context) override {
        for (auto& entity : m_Entities) {
            entity->Update(context.m_Timestep);
        }
    }

    void OnRender([[maybe_unused]] const ZPG::RenderContext& context) override {
        using namespace ZPG;

        for (auto& entity : m_Entities) {
            Renderer::Submit(*m_Scene->GetResourceManager().GetShaderProgram("basic_normal_color"), *entity);
        }
    }
private:
    ZPG::Ref<ZPG::Model> m_Model;
    std::vector<ZPG::Scope<ZPG::Entity>> m_Entities;
};



class BushesLayer : public ZPG::Layer {
public:
    BushesLayer() {
    }

    void OnAttach() override {
        using namespace ZPG;

        auto VBO = VertexBuffer::Create(bushes, sizeof(bushes), {
            {ShaderDataType::Float3, "a_Pos"},
            {ShaderDataType::Float3, "a_Normal"},
        });

        auto VAO = VertexArray::Create({ VBO });
        auto mesh = CreateRef<Mesh>(VAO);
        m_Model = CreateRef(new Model({ mesh }));

        for (int i = 0; i < 500; i++) {
            auto transform = CreateRef<Translate>(
                10.f * glm::vec3(Utility::GetRandomFloat(), 0.0f, Utility::GetRandomFloat())
            );

            m_Entities.push_back(CreateScope(new Entity(m_Model, transform)));
        }
    }

    void OnUpdate(ZPG::SceneContext& context) override {
        for (auto& entity : m_Entities) {
            entity->Update(context.m_Timestep);
        }
    }

    void OnRender([[maybe_unused]] const ZPG::RenderContext& context) override {
        using namespace ZPG;

        for (auto& entity : m_Entities) {
            Renderer::Submit(*m_Scene->GetResourceManager().GetShaderProgram("basic_normal_color"), *entity);
        }
    }
private:
    ZPG::Ref<ZPG::Model> m_Model;
    std::vector<ZPG::Scope<ZPG::Entity>> m_Entities;
};


class ForestScene : public ZPG::Scene {
public:
    ForestScene() : m_CameraController(GetCamera()) {
        
    }

    void OnAttach() override {
        PushLayer(new TreesLayer());
        PushLayer(new BushesLayer());
    }

    void OnUpdate([[maybe_unused]] ZPG::Timestep ts) {
        m_CameraController.OnUpdate(ts);
        Scene::OnUpdate(ts);
    }

    void OnEvent([[maybe_unused]] ZPG::Event& event) {
        m_CameraController.OnEvent(event);
        Scene::OnEvent(event);
    }
private:
    ZPG::CameraController m_CameraController;
};

}