#pragma once
#include "ZPGine.h"
#include "Models/box.h"
#include "AxisLayer.h"

namespace AxisSceneNS {

class AxisScene : public ZPG::Scene {
public:
    AxisScene()
    : m_CameraController(GetCamera()) {
    }

    void OnAttach() override {
        PushLayer(new AxisLayer());
    }
    void OnUpdate(ZPG::Timestep ts) override {
        Scene::OnUpdate(ts);
    }
    void OnEvent(ZPG::Event &event) override {
        Scene::OnEvent(event);
    }
private:
    ZPG::CameraController m_CameraController;
    ZPG::Ref<ZPG::SpotLight> m_FlashLight;
};

}
