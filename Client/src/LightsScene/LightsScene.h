#pragma once
#include "ZPGine.h"

class LightsScene : public ZPG::Scene {
public:
    LightsScene();
    void OnAttach() override;
    void OnUpdate(ZPG::Timestep ts) override;
    void OnEvent(ZPG::Event &event) override;
private:
    ZPG::CameraController m_CameraController;
    ZPG::Ref<ZPG::SpotLight> m_FlashLight;
};
