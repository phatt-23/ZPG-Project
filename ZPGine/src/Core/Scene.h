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
#include "Light/Light.h"

namespace ZPG {

class Scene {
public:
    virtual ~Scene() {}

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnResume();
    virtual void OnPause();
    virtual void OnUpdate(Timestep ts) { UpdateLayers(ts); }
    virtual void OnRender(Timestep ts) { RenderLayers(ts); }
    virtual void OnEvent(Event& event) { PropagateEventDownLayers(event); }
    virtual void OnImGuiRender();

    void PushLayer(Layer* layer);
    void PopLayer();
protected:
    void PropagateEventDownLayers(Event& event); 
    void UpdateLayers(Timestep ts);
    void RenderLayers(Timestep ts);
    Camera& GetCamera() { return m_Camera; }
    std::vector<Light>& GetLights() { return m_Lights; }
private:
    LayerStack m_LayerStack;
    Camera m_Camera;
    std::vector<Light> m_Lights;
};

}

#endif //SCENE_H
