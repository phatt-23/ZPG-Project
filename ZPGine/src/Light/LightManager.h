#pragma once

namespace ZPG {

class Light;

constexpr u32 ZPG_LIGHT_UNIFORM_BUFFER_ARRAY_LENGTH = 100;

class LightManager {
public:
    LightManager();
    ~LightManager();

    void AddLight(const ref<Light>& light);
    void RemoveLight(const ref<Light>& light);

    const std::vector<ref<Light>>& GetLights() const;

    std::vector<ref<Light>>::iterator begin();
    std::vector<ref<Light>>::iterator end();
private:
    std::vector<ref<Light>> m_Lights;
};

}
