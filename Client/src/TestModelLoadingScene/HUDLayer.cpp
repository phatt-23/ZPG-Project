#include "HUDLayer.h"

using namespace ZPG;

namespace TestModelLoadingNS {

void HUDLayer::OnImGuiRender() {
    ImGui::Begin("HUD");
    ImGui::Text("Position: %f, %f, %f", m_CameraPosition[0], m_CameraPosition[1], m_CameraPosition[2]);
    ImGui::Text("Orientation: %f, %f, %f", m_CameraOrientation[0], m_CameraOrientation[1], m_CameraOrientation[2]);
    ImGui::End();
}

void HUDLayer::OnNotify(ZPG::Payload &payload) {
    using namespace ZPG;
    if (payload.Type == CameraPositionChanged) {
        m_CameraPosition = *(glm::vec3*)payload.Value;
    }
    else if (payload.Type == CameraOrientationChanged) {
        m_CameraOrientation = *(glm::vec3*)payload.Value;
    }
}

}
