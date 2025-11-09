#include "LightManager.h"

#include "Debug/Logger.h"
#include "Light/Light.h"
#include "Profiling/Instrumentor.h"

namespace ZPG {

const u32 LightManager::s_LightCapacity = 256;

LightManager::LightManager() : m_Lights() {
}

LightManager::~LightManager() {
}

void LightManager::AddLight(Light* light) {
    ZPG_PROFILE_FUNCTION();
    if (m_Lights.size() >= s_LightCapacity) {
        ZPG_CORE_WARN("Light manager cannot hold more than {} lights", s_LightCapacity);
        return;
    }

    m_Lights.push_back(MakeRef(light));
}

void LightManager::AddLight(const ref<Light>& light) {
    ZPG_PROFILE_FUNCTION();
    // ZPG_CORE_ASSERT(m_Lights.size() <= ZPG_LIGHT_UNIFORM_BUFFER_ARRAY_LENGTH,
    //     "Light manager cannot hold more than {} lights", ZPG_LIGHT_UNIFORM_BUFFER_ARRAY_LENGTH);
    if (m_Lights.size() >= s_LightCapacity) {
        ZPG_CORE_WARN("Light manager cannot hold more than {} lights", s_LightCapacity);
        return;
    }

    m_Lights.push_back(light);
}
void LightManager::RemoveLight(const ref<Light>& light) {
    ZPG_PROFILE_FUNCTION();
    auto iter = std::ranges::find_if(m_Lights, [&](const ref<Light>& e){
        return (void*)light.get() == (void*)e.get();
    });

    if (iter != m_Lights.end()) {
        m_Lights.erase(iter);
        return;
    }

    ZPG_CORE_WARN("Light wasn't found in the LightArray.");
}


const std::vector<ref<Light>>& LightManager::GetLights() const {
    ZPG_PROFILE_FUNCTION();
    return m_Lights;
}
std::vector<ref<Light>>::iterator LightManager::begin() {
    return m_Lights.begin();
}
std::vector<ref<Light>>::iterator LightManager::end() {
    return m_Lights.end();
}

}
