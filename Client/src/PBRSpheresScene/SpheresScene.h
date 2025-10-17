//
// Created by phatt on 10/16/25.
//

#pragma once

#include "ZPGine.h"

namespace PBRSpheresSceneNS {

class SpheresScene : public ZPG::Scene {
public:
    SpheresScene();
    void OnAttach() override;
    void OnUpdate(ZPG::Timestep& ts) override;
    void OnEvent(ZPG::Event& event) override;
    void OnImGuiRender() override;
private:
    ZPG::CameraController m_CameraController;
};

} // BallsSceneNS
