#pragma once

namespace ZPG {

class Light;


class LightManager {
public:
    LightManager();
    ~LightManager();

    void AddLight(Light* light);
    void AddLight(const ref<Light>& light);
    void RemoveLight(const ref<Light>& light);

    const std::vector<ref<Light>>& GetLights() const;

    std::vector<ref<Light>>::iterator begin();
    std::vector<ref<Light>>::iterator end();

public:
    static const u32 s_LightCapacity;

private:
    std::vector<ref<Light>> m_Lights;

};

}
