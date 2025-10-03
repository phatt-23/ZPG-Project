#include "LightManager.h"

namespace ZPG {

LightManager::LightManager() : m_Lights(ZPG_LIGHT_UNIFORM_BUFFER_ARRAY_LENGTH, nullptr) {
    for (size_t i = 0; i < m_Lights.size(); i++) {
        m_Lights[i] = nullptr;
    }
}
LightManager::~LightManager() {
}
void LightManager::AddLight(const Ref<Light>& light) {
    for (size_t i = 0; i < m_Lights.size(); i++) {
        if (m_Lights[i] == nullptr) {
            m_Lights[i] = light;
            return;
        }
    }
    ZPG_UNREACHABLE("The light array is filled to the max");
}
void LightManager::RemoveLight(const Ref<Light>& light) {
    for (size_t i = 0; i < m_Lights.size(); i++) {
        if (m_Lights[i] == light) {
            m_Lights[i] = nullptr;
            return;
        }
    }
    ZPG_CORE_WARN("Light wasn't found in the LightArray.");
}

}
