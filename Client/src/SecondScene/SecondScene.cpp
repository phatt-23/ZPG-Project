#include "SecondScene.h"
#include "TreeLayer.h"
#include "SphereLayer.h"
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

using namespace ZPG;


SecondScene::SecondScene() : m_CameraController(), m_Timestep(0.f) {
    f32 aspectRatio = Application::Get().GetWindow().GetAspectRatio();
    Camera& camera = m_CameraController.GetCamera();
    camera.SetPerspectiveProjection(camera.GetFOV(), aspectRatio, 0.001f, camera.GetZFar());
    camera.SetPosition(glm::vec3(0.0, 0.0, 1.0));
    camera.SetOrientation(glm::quatLookAt(glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0)));
}
SecondScene::~SecondScene() {
}
void SecondScene::OnAttach() {
    // PushLayer(new TreeLayer(m_CameraController));
    PushLayer(new SphereLayer(m_CameraController));
}
void SecondScene::OnUpdate(Timestep ts) {
    m_Timestep = ts;

    // If ImGui wants keyboard, don't let controller "move"
    if (not ImGui::GetIO().WantCaptureKeyboard) 
        m_CameraController.OnUpdate(ts);

    UpdateLayers(ts);
}
void SecondScene::OnEvent(Event &event) {
    m_CameraController.OnEvent(event);
    PropagateEventDownLayers(event);
}
void SecondScene::OnImGuiRender() {
    ImGui::Begin("FirstScene Data");
        ImGui::Text("FPS: %.4f", 1.f/m_Timestep);
        glm::vec3 cPos = m_CameraController.GetCamera().GetPosition();
        glm::quat cOr = m_CameraController.GetCamera().GetOrientation();
        ImGui::InputFloat3("Camera position", glm::value_ptr(cPos));
        ImGui::InputFloat4("Camera orientation", glm::value_ptr(cOr));
    ImGui::End();
}
