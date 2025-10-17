#pragma once
#include "Light.h"
#include "Debug/Asserter.h"


namespace ZPG {

constexpr u32 ZPG_LIGHT_UNIFORM_BUFFER_ARRAY_LENGTH = 100;

class LightManager {
public:
    LightManager();
    ~LightManager();

    void AddLight(const ref<Light>& light);
    void RemoveLight(const ref<Light>& light);

    const std::vector<ref<Light>>& GetLights() const { 
        return m_Lights; 
    }

    std::vector<ref<Light>>::iterator begin() { return m_Lights.begin(); }
    std::vector<ref<Light>>::iterator end() { return m_Lights.end(); }
private:
    std::vector<ref<Light>> m_Lights;
};

}
