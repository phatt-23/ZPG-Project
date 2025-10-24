//
// Created by phatt on 9/22/25.
//

#ifndef SCENE_H
#define SCENE_H

#include "Light/LightManager.h"
#include "../Camera/Camera.h"
#include "Core/LayerStack.h"
#include "Entity/EntityManager.h"
#include "Resource/ResourceManager.h"

namespace ZPG {

class ResourceManager;
class Timestep;
class Event;

class Scene {
public:
    Scene(const ref<ResourceManager>& resourceManager = ResourceManager::GetGlobalRef());
    virtual ~Scene() {}

    // is called right away when the scene is pushed in the scene manager
    virtual void OnAttach() {}  
    // is called once when the scene is set as active
    virtual void OnLazyAttach() {}  
    virtual void OnDetach() {}
    virtual void OnResume();
    virtual void OnPause();
    virtual void OnUpdate(Timestep& ts) { UpdateLayers(ts); }
    virtual void OnRender(Timestep& ts);
    virtual void OnEvent(Event& event) { PropagateEventDownLayers(event); }
    virtual void OnImGuiRender();

    void PushLayer(Layer* layer);
    void PopLayer();

    void AddLight(Light* light);
    void RemoveLight(Light* light);

    // unnecessary, but looks nicer when working with them.
    EntityManager& GetEntityManager() { return m_EntityManager; }
    ResourceManager& GetResourceManager() { return *m_ResourceManager; }
    LightManager& GetLightManager() { return m_LightManager; }

    void MarkAsLazyLoaded() { m_LazyLoaded = true; }
    bool WasLazyLoaded() const { return m_LazyLoaded; }

protected:
    void PropagateEventDownLayers(Event& event); 
    void UpdateLayers(Timestep& ts);
    Camera& GetCamera() { return m_Camera; }

    // deprecated:
    void RenderLayers(Timestep& ts);

private:
    LayerStack m_LayerStack;
    Camera m_Camera;
    LightManager m_LightManager;
    EntityManager m_EntityManager;

    bool m_LazyLoaded = false;
protected:
    // either its own resource manager or injected by the app's resource manager that is global
    ref<ResourceManager> m_ResourceManager = nullptr;
};

}

#endif //SCENE_H
