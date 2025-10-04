#pragma once
#include "Light.h"
#include "Debug/Asserter.h"


namespace ZPG {

constexpr u32 ZPG_LIGHT_UNIFORM_BUFFER_ARRAY_LENGTH = 100;

class LightManager {
public:
    LightManager();
    ~LightManager();

    void AddLight(const Ref<Light>& light);
    void RemoveLight(const Ref<Light>& light);

    const std::vector<Ref<Light>>& GetLights() const { return m_Lights; }

    std::vector<Ref<Light>>::iterator begin() { return m_Lights.begin(); }
    std::vector<Ref<Light>>::iterator end() { return m_Lights.end(); }
private:
    std::vector<Ref<Light>> m_Lights;
};

}
