#pragma once

#include "ZPGine.h"

namespace TestTextureSceneNS {



class TestTextureScene : public ZPG::Scene {
public:
    TestTextureScene();
    void OnAttach() override;
    void OnUpdate(ZPG::Timestep ts) override;
    void OnEvent(ZPG::Event &event) override;
private:
    ZPG::CameraController m_CameraController;
};


}