#pragma once

#include "ZPGine.h"
#include "AxisScene/AxisLayer.h"
#include "ModelLayer.h"
#include "HUDLayer.h"

namespace TestModelLoadingNS {

class ModelLoadingScene : public ZPG::Scene {
public:
    ModelLoadingScene() : m_CameraController(GetCamera()) {
    }

    void OnLazyAttach() override {
        using namespace ZPG;
        // The resource manager can be 'overriden' by the client 
        // if they should choose to use their own instead of 
        // the global resource manager.
        // m_ResourceManager = ZPG::CreateScope<ZPG::ResourceManager>();
        
        m_ResourceManager->LoadModel("hyena", 
            "assets/models/hyena/hyena_demo_free_download/scene.gltf");
        m_ResourceManager->LoadModel("ford_mustang", 
            "assets/models/ford_mustang/scene.gltf");

        m_ResourceManager->LoadShaderProgram("ModelLoadingShaderProgram", 
            "./assets/shaders/vertex/DefaultLit.vert", 
            "./assets/shaders/fragment/DefaultLit.frag");

        HUDLayer* hudLayer = new HUDLayer();
        m_CameraController.AttachObserver(hudLayer);

        PushLayer(new AxisSceneNS::AxisLayer());
        PushLayer(hudLayer);
        PushLayer(new ModelLayer());

        AddLight(new AmbientLight(glm::vec4(1.0, 1.0, 1.0, 0.3)));
        AddLight(new DirectionalLight(glm::vec4(0.0, 0.0, 1.0, 0.3), glm::vec3(-1.0, -1.0, 1.0)));
        AddLight(new PointLight(glm::vec4(1.0f), glm::vec3(-1.5, 0.0, 0.0)));
        AddLight(new PointLight(glm::vec4(1.0f), glm::vec3(0.0, 1.0, -1.0)));
        AddLight(new PointLight(glm::vec4(1.0f), glm::vec3(1.5, 0.0, 0.0)));
        AddLight(new SpotLight(
            glm::vec4(1.0, 1.0, 0.0, 1.0), 
            glm::vec3(21.0, 1.0, 0.0), 
            glm::vec3(1.0, -0.5, 0.0), 32.f, 34.f));
        AddLight(new SpotLight(
            glm::vec4(1.0, 1.0, 0.0, 1.0), 
            glm::vec3(16.0, 0.0, 0.0), 
            glm::vec3(0.0, -0.5, 0.0), 32.f, 34.f));
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
    ZPG::Scope<ZPG::ResourceManager> m_LocalResourceManager;
};

}
