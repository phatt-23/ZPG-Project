#include "Scene.h"
#include "Renderer/RenderCommand.h"

namespace ZPG {

void Scene::PushLayer(Layer* layer) {
    m_LayerStack.PushLayer(layer);
}
void Scene::PopLayer() {
    m_LayerStack.PopLayer();
}
void Scene::OnImGuiRender() {
    for (auto& layer : m_LayerStack) {
        layer->OnImGuiRender();
    }
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
    RenderCommand::SetClearColor({0.0, 0.0, 0.0, 1.0});
    RenderCommand::Clear();

    for (auto& layer : m_LayerStack) {
        layer->OnUpdate(ts);
    }
}

}

