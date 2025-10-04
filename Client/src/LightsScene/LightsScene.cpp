#include "LightsScene.h"
#include "ObjectsLayer.h"
#include <imgui.h>

using namespace ZPG;
using namespace glm;

LightsScene::LightsScene() 
: m_CameraController(GetCamera()) {
}

void LightsScene::OnAttach() {
    PushLayer(new ObjectsLayer());
    m_FlashLight = CreateRef<SpotLight>( vec4(1.f, 1.f, 1.f, 1.f), vec3(0.0, -0.1, 0.0f), vec3(0.f, 1.f, 0.f), 30.f, 32.f );

    AddLight(CreateRef<AmbientLight>( vec4(1.f, 1.f, 1.f, 0.1f) ));
    // AddLight(CreateRef<PointLight>( vec4(1.f, 1.f, 0.f, 1.f), vec3(1.f, 1.f, -1.f) ));
    // AddLight(CreateRef<PointLight>( vec4(1.f, 1.f, 0.f, 1.f), vec3(0.f, -1.f, 0.f) ));
    // AddLight(CreateRef<DirectionalLight>( vec4(1.f, 1.f, 1.f, 1.f), vec3(-1.f, -1.f, -1.f) ));
    AddLight(m_FlashLight);
}
void LightsScene::OnUpdate(Timestep ts) {
    m_CameraController.OnUpdate(ts);
    m_FlashLight->SetPosition(m_CameraController.GetCamera().GetPosition());
    m_FlashLight->SetDirection(m_CameraController.GetCamera().GetFront());
    Scene::OnUpdate(ts);
}
void LightsScene::OnEvent(Event &event) {
    m_CameraController.OnEvent(event);
    Scene::OnEvent(event);
}


