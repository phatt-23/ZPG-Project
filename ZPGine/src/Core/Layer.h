//
// Created by phatt on 9/22/25.
//

#ifndef LAYER_H
#define LAYER_H

#include "Event/Event.h"
#include "Timestep.h"
#include "SceneContext.h"
#include "ResourceManager.h"

namespace ZPG {

class Scene; 

class Layer {
public:
    Layer() {} 
    virtual ~Layer() = default;
    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate([[maybe_unused]] SceneContext& ctx) {}
    virtual void OnEvent([[maybe_unused]] Event& event) {}
    virtual void OnRender([[maybe_unused]] const RenderContext& ctx) {}
    virtual void OnImGuiRender() {}

    void SetScene(Scene* scene) { 
        m_Scene = scene; 
    }
protected:
    Scene* m_Scene = nullptr; // parent scene
};

}

#endif //LAYER_H
