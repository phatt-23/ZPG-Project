#pragma once

#include "ZPGine.h"
#include "AxisScene/AxisLayer.h"
#include "ModelLayer.h"
#include "../assets/models/nemec/sphere.h"

namespace TransformHierarchyNS {

class TransformHierarchyScene : public ZPG::Scene {
public:
    TransformHierarchyScene() : m_CameraController(GetCamera()) {
    }

    void OnLazyAttach() override {
        using namespace ZPG;

        auto sphereVAO = VertexArray::Create({
            VertexBuffer::Create(nemec::sphere, sizeof(nemec::sphere), {
                {ShaderDataType::Float3, "a_Pos"},
                {ShaderDataType::Float3, "a_Normal"},
            }),
        });

        if (!m_ResourceManager->HasModel("sphere")) {
            m_ResourceManager->AddModel("sphere", Model::Create({ Mesh::Create(sphereVAO) }));
        }

        PushLayer(new AxisSceneNS::AxisLayer());
        PushLayer(new ModelLayer());

        AddLight(new AmbientLight(v4(1.0, 1.0, 1.0, 0.3)));
        AddLight(new DirectionalLight(v4(0.0, 0.0, 1.0, 0.3), v3(-1.0, -1.0, 1.0)));
        AddLight(new PointLight(v4(1.0f), v3(-1.5, 0.0, 0.0)));
        AddLight(new PointLight(v4(1.0f), v3(0.0, 1.0, -1.0)));
        AddLight(new PointLight(v4(1.0f), v3(1.5, 0.0, 0.0)));
        AddLight(new SpotLight(
            v4(1.0, 1.0, 0.0, 1.0), 
            v3(21.0, 1.0, 0.0), 
            v3(1.0, -0.5, 0.0), 32.f, 34.f));
        AddLight(new SpotLight(
            v4(1.0, 1.0, 0.0, 1.0), 
            v3(16.0, 0.0, 0.0), 
            v3(0.0, -0.5, 0.0), 32.f, 34.f));
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
    ZPG::ObservableCameraController m_CameraController;
    ZPG::scope<ZPG::ResourceManager> m_LocalResourceManager;
};

}
