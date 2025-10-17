#pragma once

#include "Core/Timestep.h"
#include "ZPGine.h"

class SecondScene : public ZPG::Scene {
public:
    SecondScene();
    ~SecondScene() override;

    void OnAttach() override;
    void OnUpdate(ZPG::Timestep& ts) override;
    void OnEvent(ZPG::Event &event) override;
    void OnImGuiRender() override;
private:
    ZPG::CameraController m_CameraController;
    ZPG::Timestep m_Timestep;
};
