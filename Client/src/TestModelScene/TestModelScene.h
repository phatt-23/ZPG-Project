#pragma once
#include "ZPGine.h"

namespace TestModelSceneNS {

class TestModelScene : public ZPG::Scene {
public:
    TestModelScene();
    void OnAttach() override;
    void OnUpdate(ZPG::Timestep& ts) override;
    void OnEvent(ZPG::Event &event) override;
private:
    ZPG::CameraController m_CameraController;
    ZPG::SpotLight* m_FlashLight;
};

}  