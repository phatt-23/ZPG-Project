#pragma once

#include "Core/Timestep.h"

namespace ZPG {

class Scene;
class Camera;
class Light;

// these are not needed if I set the layer's parent scene

// readonly stuff needed for rendering
struct RenderContext {
    Timestep Ts = 0.f;
    const Camera& Cam;
    const std::vector<ref<Light>>& Lights;
};

// APIs for modifying the world
struct SceneContext {
    Timestep Ts = 0.f;

    // Deprecated!!
    // Controlled modification APIs
    std::function<void(const Light&)> AddLight;
    // std::function<void(const Entity&)> AddEntity;
    // std::function<void(u32 entityID)> DestroyEntity;
};

}
