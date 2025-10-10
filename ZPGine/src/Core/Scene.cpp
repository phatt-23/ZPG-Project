#include "Scene.h"
#include "Renderer/RenderCommand.h"
#include "Application.h"
#include <imgui.h>

namespace ZPG {

Scene::Scene(const Ref<ResourceManager>& resourceManager)
: m_ResourceManager(resourceManager) {

}

void Scene::PushLayer(Layer* layer) {
    layer->SetScene(this);
    m_LayerStack.PushLayer(layer);
}
void Scene::PopLayer() {
    Ref<Layer> layer = m_LayerStack.PopLayer();
    layer->SetScene(nullptr);
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
void Scene::OnImGuiRender() {
    for (auto& layer : m_LayerStack) {
        layer->OnImGuiRender();
    }
}
void Scene::UpdateLayers(Timestep ts) {
    RenderCommand::SetClearColor({0.0, 0.0, 0.0, 1.0});
    RenderCommand::Clear();

    SceneContext ctx;
    ctx.m_Timestep = ts;
    ctx.AddLight = [this](const Light& light) {
        ZPG_CORE_DEBUG("Layer wants to add light.");
        return;  
    };

    for (auto& layer : m_LayerStack) {
        layer->OnUpdate(ctx);
    }
}
void Scene::RenderLayers(Timestep ts) {
    RenderContext ctx = {
        .m_Timestep = ts,
        .m_Camera = m_Camera,
        .m_Lights = m_LightManager.GetLights(),
    };

    Renderer::BeginDraw(m_Camera);
    Renderer::SetLights(m_LightManager.GetLights());
    for (auto& layer : m_LayerStack) {
        layer->OnRender(ctx);
    }
    Renderer::EndDraw();
}
void Scene::OnResume() {
    // Sending event because the scene must adapt to the current window size
    const Window& window = Application::Get().GetWindow();
    WindowResizeEvent event(window.GetWidth(), window.GetHeight());
    this->OnEvent(event);
}
void Scene::OnPause() {

}
void Scene::AddLight(const Ref<Light>& light) {
    m_LightManager.AddLight(light);
}
void Scene::RemoveLight(const Ref<Light>& light) {
    m_LightManager.RemoveLight(light);
}

}

