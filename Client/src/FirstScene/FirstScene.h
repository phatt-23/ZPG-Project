#pragma once

#include "ZPGine.h"

class FirstScene : public ZPG::Scene {
public:
    void OnAttach() override;
    void OnUpdate(ZPG::Timestep ts) override;
    void OnEvent(ZPG::Event &event) override;
private:
};
