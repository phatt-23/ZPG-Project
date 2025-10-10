#pragma once

#include "ZPGine.h"

namespace TestModelLoadingNS {

class HUDLayer : public ZPG::Layer, public ZPG::Observer {
public:
    void OnImGuiRender() override;
    void OnNotify(ZPG::Payload &payload) override;
private:
    glm::vec3 m_CameraPosition;
    glm::vec3 m_CameraOrientation;
    
};

}
