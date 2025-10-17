#include "TestModelScene.h"
#include "ObjectsLayer.h"
#include <imgui.h>

using namespace ZPG;
using namespace glm;

namespace TestModelSceneNS {

TestModelScene::TestModelScene() 
: m_CameraController(GetCamera()) {
}

void TestModelScene::OnAttach() {
    PushLayer(new ObjectsLayer());
    
    m_FlashLight = new SpotLight( vec4(1.f, 1.f, 1.f, 1.f), vec3(0.0, -0.1, 0.0f), vec3(0.f, 1.f, 0.f), 30.f, 32.f );

    AddLight(new AmbientLight( vec4(1.f, 1.f, 1.f, 0.1f) ));
    // AddLight(CreateRef<PointLight>( vec4(1.f, 1.f, 0.f, 1.f), vec3(1.f, 1.f, -1.f) ));
    // AddLight(CreateRef<PointLight>( vec4(1.f, 1.f, 0.f, 1.f), vec3(0.f, -1.f, 0.f) ));
    // AddLight(CreateRef<DirectionalLight>( vec4(1.f, 1.f, 1.f, 1.f), vec3(-1.f, -1.f, -1.f) ));
    AddLight(m_FlashLight);
}
void TestModelScene::OnUpdate(Timestep& ts) {
    m_CameraController.OnUpdate(ts);
    m_FlashLight->SetPosition(m_CameraController.GetCamera().GetPosition());
    m_FlashLight->SetDirection(m_CameraController.GetCamera().GetFront());
    Scene::OnUpdate(ts);
}
void TestModelScene::OnEvent(Event &event) {
    m_CameraController.OnEvent(event);
    Scene::OnEvent(event);
}

}
