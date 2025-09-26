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
    virtual void OnUpdate(Timestep ts) {}
    virtual void OnEvent(Event& event) {}
private:
};

}

#endif //LAYER_H
