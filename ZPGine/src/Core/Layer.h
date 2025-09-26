//
// Created by phatt on 9/22/25.
//

#ifndef LAYER_H
#define LAYER_H

#include "Event/Event.h"
#include "Timestep.h"

namespace ZPG {

class Layer {
public:
    Layer() = default;
    virtual ~Layer() = default;
    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate([[maybe_unused]] Timestep ts) {}
    virtual void OnEvent([[maybe_unused]] Event& event) {}
private:
};

}

#endif //LAYER_H
