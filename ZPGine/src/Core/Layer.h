//
// Created by phatt on 9/22/25.
//

#ifndef LAYER_H
#define LAYER_H

#include "Event/Event.h"
#include "Timestep.h"
#include "SceneContext.h"

namespace ZPG {

class Layer {
public:
    Layer() = default;
    virtual ~Layer() = default;
    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate([[maybe_unused]] SceneContext& ctx) {}
    virtual void OnEvent([[maybe_unused]] Event& event) {}
    virtual void OnRender([[maybe_unused]] const RenderContext& ctx) {}
    virtual void OnImGuiRender() {}
private:
};

}

#endif //LAYER_H
