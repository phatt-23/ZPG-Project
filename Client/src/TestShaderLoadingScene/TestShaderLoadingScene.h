#pragma once
#include "ZPGine.h"
#include "Models/gift.h"
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include "InlineShadersLayer.h"
#include "ReadingFromFilesLayer.h"

using namespace ZPG;

class TestShaderLoadingScene : public Scene {
public:
    TestShaderLoadingScene() : m_CameraController(), m_Timestep(0.f) {
        f32 aspectRatio = Application::Get().GetWindow().GetAspectRatio();
        Camera& camera = m_CameraController.GetCamera();
        camera.SetPerspectiveProjection(camera.GetFOV(), aspectRatio, 0.001f, camera.GetZFar());
        camera.SetPosition(glm::vec3(0.0, 0.0, 1.0));
        camera.SetOrientation(glm::quatLookAt(glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0)));
    }
    void OnAttach() override {
        PushLayer(new InlineShadersLayer(m_CameraController));
        PushLayer(new ReadingFromFilesLayer(m_CameraController));
    }
    void OnUpdate(Timestep ts) override {
        m_Timestep = ts;
        if (not ImGui::GetIO().WantCaptureKeyboard) 
            m_CameraController.OnUpdate(ts);
        UpdateLayers(ts);
    }
    void OnEvent(Event& event) override {
        m_CameraController.OnEvent(event);
    }
private:
    Ref<ShaderProgram> m_ShaderProgram;
    CameraController m_CameraController;
    Ref<VertexArray> m_TreeVAO;
    Timestep m_Timestep;
};
