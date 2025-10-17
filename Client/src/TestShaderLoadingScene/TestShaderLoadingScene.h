#pragma once
#include "ZPGine.h"
#include <glm/gtc/type_ptr.hpp>
#include "InlineShadersLayer.h"
#include "ReadingFromFilesLayer.h"

using namespace ZPG;

class TestShaderLoadingScene : public Scene {
public:
    TestShaderLoadingScene() 
    : m_CameraController(GetCamera())
    , m_Timestep(0.f) {
        f32 aspectRatio = Application::Get().GetWindow().GetAspectRatio();
        Camera& camera = m_CameraController.GetCamera();
        camera.SetPerspectiveProjection(camera.GetFOV(), aspectRatio, 0.001f, camera.GetZFar());
        camera.SetPosition(glm::vec3(0.0, 0.0, 1.0));
        camera.SetOrientation(glm::quatLookAt(glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0)));
    }
    void OnAttach() override {
        PushLayer(new InlineShadersLayer());
        PushLayer(new ReadingFromFilesLayer());
    }
    void OnUpdate(Timestep ts) override {
        m_Timestep = ts;
        m_CameraController.OnUpdate(ts);
        Scene::OnUpdate(ts);
    }
    void OnEvent(Event& event) override {
        m_CameraController.OnEvent(event);
        Scene::OnEvent(event);
    }
private:
    ref<ShaderProgram> m_ShaderProgram;
    CameraController m_CameraController;
    ref<VertexArray> m_TreeVAO;
    Timestep m_Timestep;
};
