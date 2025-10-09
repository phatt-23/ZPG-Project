#pragma once

#include "Core/Core.h"
#include "Light/PointLight.h"
#include "Transform/CompoundTransform.h"
#include "ZPGine.h"
#include "Models/box.h"
#include "AxisScene/AxisLayer.h"

namespace TestModelLoadingNS {

class ModelLayer : public ZPG::Layer {
public:
    ModelLayer() {
    }

    void OnAttach() override {
        using namespace ZPG;

        m_BoxVAO = VertexArray::Create({
            VertexBuffer::Create(BoxModel::boxVertices, sizeof(BoxModel::boxVertices), {
                {ShaderDataType::Float3, "a_Pos"},
                {ShaderDataType::Float3, "a_Normal"},
                {ShaderDataType::Float2, "a_TexCoord"},
            }),
        }, IndexBuffer::Create(BoxModel::boxIndices, ZPG_ARRAY_LENGTH(BoxModel::boxIndices)));

        auto transform = CompoundTransform::Create();
        transform->Emplace<ScaleTransform>(0.01f * glm::vec3(1.0f));
        transform->Emplace<TranslationTransform>(glm::vec3(0.0f, 0.8f, 0.0f));
        transform->Emplace<RotationTransform>(180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        transform->Emplace<DynRotationTransform>(0.0f, 20.0f, glm::vec3(0.0, 1.0, 0.0));
        transform->Emplace<DynRotationTransform>(0.0f, 20.0f, glm::vec3(1.0, 0.0, 0.0));

        auto transform2 = CompoundTransform::Create();
        transform2->Emplace<ScaleTransform>(0.01f * glm::vec3(1.0f));
        transform2->Emplace<TranslationTransform>(glm::vec3(0.0f, 0.8f, 3.0f));
        transform2->Emplace<RotationTransform>(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        transform2->Emplace<DynRotationTransform>(0.0f, 50.0f, glm::vec3(0.0, 1.0, 0.0));

        m_Entities.push_back(CreateScope<Entity>(
            m_ResourceManager.GetModel("hyena"), transform));
        m_Entities.push_back(CreateScope<Entity>(
            m_ResourceManager.GetModel("hyena"), transform2));
    }

    void OnUpdate(ZPG::SceneContext& context) override {
        for (auto& entity : m_Entities) {
            entity->Update(context.m_Timestep);
        }
    }

    void OnRender(const ZPG::RenderContext& context) override {
        using namespace ZPG;

        Renderer::BeginDraw(context.m_Camera);
        Renderer::SetLights(context.m_Lights);
        for (auto& entity : m_Entities) {
            Renderer::SumbitEntity(*entity);
        }
        for (auto& light : context.m_Lights) {
            if (light->GetLightType() == ZPG::LightType::Point) {
                auto pointLight = (ZPG::PointLight*)light.get();

                auto transform = CompoundTransform::Create();
                transform->Emplace<ScaleTransform>(0.4f * glm::vec3(1.0));
                transform->Emplace<TranslationTransform>(pointLight->GetPosition());

                Renderer::Submit(
                    *m_ResourceManager.GetShaderProgram("basic_single_color"),
                    *m_BoxVAO,
                    transform->GetMatrix()
                );
            }
        }
        Renderer::EndDraw();
    }
private:
    std::vector<ZPG::Scope<ZPG::Entity>> m_Entities;
    ZPG::Ref<ZPG::VertexArray> m_BoxVAO;
};


class ModelLoadingScene : public ZPG::Scene {
public:
    ModelLoadingScene() : m_CameraController(GetCamera()) {
    }

    void OnAttach() override {
        using namespace ZPG;
        // The resource manager can be 'overriden' by the client if they should choose to use their 
        // own instead of the global resource manager.
        // m_LocalResourceManager = ZPG::CreateScope<ZPG::ResourceManager>();
        m_ResourceManager.LoadModel("hyena", "assets/models/hyena/hyena_demo_free_download/scene.gltf");
        // m_ResourceManager.LoadModel("ford_mustang", "assets/models/ford_mustang/scene.gltf");

        m_ResourceManager.LoadShaderProgram("ModelLoadingShaderProgram", 
            "./assets/shaders/vertex/DefaultLit.vert", 
            "./assets/shaders/fragment/DefaultLit.frag");
        PushLayer(new ModelLayer());
        PushLayer(new AxisSceneNS::AxisLayer());

        AddLight(CreateRef(new AmbientLight(glm::vec4(1.0, 1.0, 1.0, 0.1))));
        AddLight(CreateRef(new PointLight(glm::vec4(1.0f), glm::vec3(-1.5, 0.0, 0.0))));
        AddLight(CreateRef(new PointLight(glm::vec4(1.0f), glm::vec3(0.0, 1.0, -1.0))));
        AddLight(CreateRef(new PointLight(glm::vec4(1.0f), glm::vec3(1.5, 0.0, 0.0))));
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
