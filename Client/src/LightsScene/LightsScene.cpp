#include "LightsScene.h"
#include "BoxesLayer.h"
#include <imgui.h>

using namespace ZPG;

LightsScene::LightsScene() : m_CameraController(GetCamera()) {
}
void LightsScene::OnAttach() {
    PushLayer(new BoxesLayer());

    std::vector<Light>& lights = GetLights();

    lights.emplace_back<>();
}
void LightsScene::OnUpdate(ZPG::Timestep ts) {
    m_CameraController.OnUpdate(ts);
    Scene::OnUpdate(ts);
}
void LightsScene::OnEvent(ZPG::Event &event) {
    m_CameraController.OnEvent(event);
    Scene::OnEvent(event);
}


