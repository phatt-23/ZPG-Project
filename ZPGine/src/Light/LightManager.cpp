#include "LightManager.h"

namespace ZPG {

LightManager::LightManager() : m_Lights() {
}

LightManager::~LightManager() {
}

void LightManager::AddLight(const Ref<Light>& light) {
    ZPG_CORE_ASSERT(m_Lights.size() <= ZPG_LIGHT_UNIFORM_BUFFER_ARRAY_LENGTH, 
        "Light manager cannot hold more than {} lights", ZPG_LIGHT_UNIFORM_BUFFER_ARRAY_LENGTH);

    m_Lights.push_back(light);
}
void LightManager::RemoveLight(const Ref<Light>& light) {
    auto iter = std::find_if(m_Lights.begin(), m_Lights.end(), [&](const Ref<Light>& e){
        return (void*)light.get() == (void*)e.get();
    });

    if (iter != m_Lights.end()) {
        m_Lights.erase(iter);
        return;
    }

    ZPG_CORE_WARN("Light wasn't found in the LightArray.");
}

}
