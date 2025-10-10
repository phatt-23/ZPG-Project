#pragma once

#include "Renderer/Camera.h"
#include "Light/Light.h"
#include "Core/Timestep.h"

namespace ZPG {

// these are not needed if I set the layer's parent scene

// readonly stuff needed for rendering
struct RenderContext {
    Timestep m_Timestep = 0.f;
    const Camera& m_Camera;
    const std::vector<Ref<Light>>& m_Lights;
};

// APIs for modifying the world
struct SceneContext {
    Timestep m_Timestep = 0.f;
    // Controlled modification APIs
    std::function<void(const Light&)> AddLight;
    // std::function<void(const Entity&)> AddEntity;
    // std::function<void(u32 entityID)> DestroyEntity;
};

}
