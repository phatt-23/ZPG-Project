#include "Scene.h"

namespace ZPG {

void Scene::PushLayer(Layer* layer) {
    m_LayerStack.PushLayer(layer);
}

void Scene::PopLayer() {
    m_LayerStack.PopLayer();
}

void Scene::PropagateEventDownLayers(Event& event) {
    // events travel from top to bottom
    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
        (*--it)->OnEvent(event);
        // if this layer has handled the event, don't propagate the event further down the layers
        if (event.IsHandled())  
            break;
    }
}

void Scene::UpdateLayers(Timestep ts) {
    for (auto& layer : m_LayerStack) {
        layer->OnUpdate(ts);
    }
}

}

