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
class Sky;
class CameraController;

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
     * By default, the scene assumes the Layering workflow.
     */

    virtual void OnUpdate(Timestep& ts);
    // virtual void OnRender(Timestep& ts);
    virtual void OnEvent(Event& event);
    virtual void OnImGuiRender();

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
    
    const ResourceManager& GetResourceManager() const { return *m_ResourceManager; }
    const EntityManager& GetEntityManager() const { return m_EntityManager; }
    const LightManager& GetLightManager() const { return m_LightManager; }

    /**
     * Lazy load.
     */

    void MarkAsLazyLoaded() { m_AlreadyLazyLoaded = true; }
    bool WasLazyLoaded() const { return m_AlreadyLazyLoaded; }

    /**
     * Camera.
     */

    Camera& GetCamera() { return m_Camera; }
    const Camera& GetCamera() const { return m_Camera; }

    const ref<CameraController>& GetCameraController() const;
    void SetCameraController(const ref<CameraController>& cameraController);

    /**
    *Sky
    */
    void SetSky(const ref<Sky>& sky);
    const ref<Sky>& GetSky() const;

   private:
    LayerStack m_LayerStack;
    Camera m_Camera;
    LightManager m_LightManager;
    EntityManager m_EntityManager;

    /**
     * Basic camera controller. Can be replaced.
     */
    ref<CameraController> m_CameraController;

    /**
     * Either its own resource manager or 
     * injected by the application's global resource manager.
     */

    ref<ResourceManager> m_ResourceManager = nullptr;

    ref<Sky> m_Sky = nullptr;

    bool m_AlreadyLazyLoaded = false;  // has it already been lazy loaded?

protected:
    /**
     * Layering workflow helper methods
     */

    void PropagateEventDownLayers(Event& event);
    void ImGuiRenderEachLayer();

    /**
     * Entities helper methods. Scene centric.
     */

    void ImGuiRenderDebug();
};

}

#endif //SCENE_H
