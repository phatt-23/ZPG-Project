//
// Created by phatt on 9/22/25.
//

#ifndef SCENE_H
#define SCENE_H

#include "Event/Event.h"
#include "Light/LightManager.h"
#include "Renderer/Camera.h"
#include "Shader/ShaderProgramLibrary.h"
#include "Timestep.h"
#include "LayerStack.h"
#include "Light/Light.h"
#include "Core/ResourceManager.h"
#include "Entity/EntityManager.h"

namespace ZPG {

class Scene {
public:
    Scene(const Ref<ResourceManager>& resourceManager = ResourceManager::GetGlobalRef());
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

    void AddLight(const Ref<Light>& light);
    void RemoveLight(const Ref<Light>& light);

    EntityManager& GetEntityManager() { return m_EntityManager; }
    ResourceManager& GetResourceManager() { return *m_ResourceManager; }
protected:
    void PropagateEventDownLayers(Event& event); 
    void UpdateLayers(Timestep ts);
    void RenderLayers(Timestep ts);
    Camera& GetCamera() { return m_Camera; }
private:
    LayerStack m_LayerStack;
    Camera m_Camera;
    LightManager m_LightManager;
    EntityManager m_EntityManager;
protected:
    // either its own resource manager or injected by the app's resource manager that is global
    Ref<ResourceManager> m_ResourceManager = nullptr;
};

}

#endif //SCENE_H
