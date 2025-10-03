#include "LightsScene.h"
#include "BoxesLayer.h"
#include "Light/DirectionalLight.h"
#include "Light/SpotLight.h"
#include <imgui.h>

using namespace ZPG;
using namespace glm;

LightsScene::LightsScene() : m_CameraController(GetCamera()) {
}
void LightsScene::OnAttach() {
    PushLayer(new BoxesLayer());

    AddLight(CreateRef<AmbientLight>( vec4(1.f, 1.f, 1.f, 0.1f) ));
    AddLight(CreateRef<PointLight>( vec4(1.f, 1.f, 0.f, 1.f), vec3(1.f, 1.f, 1.f) ));
    AddLight(CreateRef<PointLight>( vec4(1.f, 1.f, 0.f, 1.f), vec3(-1.f, -1.f, -1.f) ));
    AddLight(CreateRef<DirectionalLight>( vec4(1.f, 1.f, 1.f, 1.f), vec3(-1.f, -1.f, -1.f) ));
    AddLight(CreateRef<SpotLight>( vec4(1.f), vec3(2.f), vec3(-1.f, -1.f, -1.f), 30.f, 35.f ));
}
void LightsScene::OnUpdate(Timestep ts) {
    m_CameraController.OnUpdate(ts);
    Scene::OnUpdate(ts);
}
void LightsScene::OnEvent(Event &event) {
    m_CameraController.OnEvent(event);
    Scene::OnEvent(event);
}


