//
// Created by phatt on 9/22/25.
//

#ifndef SCENE_H
#define SCENE_H

#include "Event/Event.h"
#include "Renderer/Camera.h"
#include "Timestep.h"
#include "LayerStack.h"
#include "Debug/Logger.h"

namespace ZPG {

class Scene {
public:
    virtual void OnUpdate(Timestep ts) {
        UpdateLayers(ts);
    }

    virtual void OnEvent(Event& event) {
        PropagateEventDownLayers(event);
    }
    virtual void OnAttach() {}
    virtual void OnDetach() {}

    void PushLayer(Layer* layer);
    void PopLayer();
protected:
    void PropagateEventDownLayers(Event& event); 
    void UpdateLayers(Timestep ts);
private:
    LayerStack m_LayerStack;
    // Camera m_Camera;
};

}

#endif //SCENE_H
