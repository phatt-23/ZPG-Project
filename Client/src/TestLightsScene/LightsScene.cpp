#include "LightsScene.h"
#include "ObjectsLayer.h"
#include "LightObjectsLayer.h"

using namespace ZPG;
using namespace glm;

LightsScene::LightsScene() 
: m_CameraController(GetCamera()) {
}

void LightsScene::OnAttach() {
    m_FlashLight = new SpotLight( vec4(0.f, 0.f, 0.f, 1.f), vec3(0.0, -0.1, 0.0f), vec3(0.f, 1.f, 0.f), 30.f, 32.f );

    PushLayer(new ObjectsLayer());
    PushLayer(new LightObjectsLayer());

    // AddLight(CreateRef<AmbientLight>( vec4(1.f, 1.f, 1.f, 0.1f) ));
    AddLight(new PointLight( vec4(0.f, 0.f, 1.f, 1.f), vec3(1.f, 0.f, -1.f) ));
    AddLight(new PointLight( vec4(0.f, 1.f, 0.f, 1.f), vec3(0.f, 0.f, 1.f) ));
    // AddLight(CreateRef<DirectionalLight>( vec4(1.f, 1.f, 1.f, 1.f), vec3(-1.f, -1.f, -1.f) ));
    AddLight(m_FlashLight);
}
void LightsScene::OnUpdate(Timestep& ts) {
    m_CameraController.OnUpdate(ts);
    m_FlashLight->m_Position.SetPosition(m_CameraController.GetCamera().GetPosition());
    m_FlashLight->m_Direction.SetDirection(m_CameraController.GetCamera().GetFront());
    Scene::OnUpdate(ts);
}
void LightsScene::OnEvent(Event &event) {
    m_CameraController.OnEvent(event);
    Scene::OnEvent(event);
}


