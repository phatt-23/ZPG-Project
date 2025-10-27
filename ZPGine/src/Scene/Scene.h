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
    virtual ~Scene();

    /**
     * Lifetime hook-in methods.
     */

    virtual void OnAttach();        // called right when the scene is pushed in the scene manager
    virtual void OnLazyAttach();    // called once when the scene is set as active
    virtual void OnDetach();
    virtual void OnResume();            
    virtual void OnPause(); 

    /**
     * Main methods.
     * By default the scene assumes the Layering workflow.
     */

    virtual void OnUpdate(Timestep& ts) { UpdateEachLayer(ts); UpdateEntities(ts); }
    virtual void OnRender(Timestep& ts) { RenderEachLayer(ts); RenderEntities(ts); }
    virtual void OnEvent(Event& event) { PropagateEventDownLayers(event); }
    virtual void OnImGuiRender() { ImGuiRenderEachLayer(); ImGuiRenderDebug(); }

    /**
     * Layering workflow.
     */

    void PushLayer(Layer* layer);
    void PopLayer();

    /**
     * Helpers for flattening the code.
     */

    void AddLight(Light* light);        
    void AddLight(const ref<Light>& light);
    void RemoveLight(Light* light);

    void AddEntity(Entity* entity);
    void AddEntity(const ref<Entity>& entity);
    void RemoveEntity(Entity* entity);

    /**
     * Scene's various object managers.
     */

    ResourceManager& GetResourceManager() { return *m_ResourceManager; }
    EntityManager& GetEntityManager() { return m_EntityManager; }
    LightManager& GetLightManager() { return m_LightManager; }

    /**
     * Lazy load.
     */

    void MarkAsLazyLoaded() { m_AlreadyLazyLoaded = true; }
    bool WasLazyLoaded() const { return m_AlreadyLazyLoaded; }

    /**
     * Camera.
     */

    Camera& GetCamera() { return m_Camera; }

private:
    LayerStack m_LayerStack;
    Camera m_Camera;
    LightManager m_LightManager;
    EntityManager m_EntityManager;

    /**
     * Either its own resource manager or 
     * injected by the application's global resource manager.
     */

    ref<ResourceManager> m_ResourceManager = nullptr;

    bool m_AlreadyLazyLoaded = false;  // has it already been lazy loaded?

protected:
    /**
     * Layering workflow helper methods
     */

    void UpdateEachLayer(Timestep& ts);
    void RenderEachLayer(Timestep& ts);
    void PropagateEventDownLayers(Event& event); 
    void ImGuiRenderEachLayer();

    /**
     * Entities helper methods. Scene centric.
     */

    void UpdateEntities(Timestep& ts);
    void RenderEntities(Timestep& ts);
    void ImGuiRenderDebug();
};

}

#endif //SCENE_H
