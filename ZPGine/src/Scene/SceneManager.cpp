#include "SceneManager.h"
#include "Debug/Asserter.h"
#include "Scene.h"
#include "Observer/Payload.h"
#include "Profiling/Instrumentor.h"

namespace ZPG 
{

    SceneManager::SceneManager() 
    {
    }

    SceneManager::~SceneManager() 
    {
        ZPG_PROFILE_FUNCTION();
        for (auto& sceneEntry : m_SceneEntries | std::views::values) 
        {
            if (sceneEntry.m_Scene == nullptr) {
                continue;
            }

            sceneEntry.m_Scene->OnDetach();
            delete sceneEntry.m_Scene;

            sceneEntry.m_Scene = nullptr;
        }
    }

    void SceneManager::AddScene(const std::string& name, const std::function<Scene*(void)>& sceneFactory, SceneLifetime lifetime) 
    {
        ZPG_PROFILE_FUNCTION();
        ZPG_CORE_DEBUG("SceneManager added scene with name {}", name);
        ZPG_CORE_ASSERT(!Exists(name), "Scene with the given name already exists.");

        SceneEntry entry;
        entry.m_SceneFactory = sceneFactory;
        entry.m_Lifetime = lifetime;

        m_SceneEntries[name] = entry;

        // set this scene as active if the current active scene name doesn't point to any scene
        if (!Exists(m_ActiveSceneName)) {
            SetActiveScene(name);
        }
    }

    void SceneManager::AddScene(const std::string& name, Scene* scene) 
    {
        ZPG_PROFILE_FUNCTION();
        ZPG_CORE_DEBUG("SceneManager added scene with name {}", name);
        ZPG_CORE_ASSERT(!Exists(name), "Scene with the given name already exists.");

        SceneEntry entry;
        entry.m_Scene = scene;
        scene->OnAttach();
        entry.m_Lifetime = SceneLifetime::Persistent;

        m_SceneEntries[name] = entry;

        // set this scene as active if the current active scene name doesn't point to any scene
        if (!Exists(m_ActiveSceneName)) {
            SetActiveScene(name);
        }
    }

    bool SceneManager::RemoveScene(const std::string& name) 
    {
        ZPG_PROFILE_FUNCTION();
        if (Exists(name)) 
        {
            Scene* scene = m_SceneEntries[name].m_Scene;
            m_SceneEntries.erase(name);
            scene->OnDetach();
            delete scene;
            return true;
        }
        return false;
    }

    bool SceneManager::Exists(const std::string& name) const 
    {
        ZPG_PROFILE_FUNCTION();
        return m_SceneEntries.contains(name);
    }

    void SceneManager::SetActiveScene(const std::string& name) 
    {
        ZPG_PROFILE_FUNCTION();
        ZPG_CORE_ASSERT(Exists(name), "Cannot set scene with name '{}' as active, it doesn't exist.", name);

        if (name == m_ActiveSceneName) {
            return;
        }

        if (Exists(m_ActiveSceneName)) 
        {
            SceneEntry& previousEntry = m_SceneEntries[m_ActiveSceneName];
            previousEntry.m_Scene->OnPause();

            // if the previous scene is transient, delete it
            if (previousEntry.m_Lifetime == SceneLifetime::Transient) 
            {
                ZPG_CORE_DEBUG("Deleting transient scene with name {}", m_ActiveSceneName);

                previousEntry.m_Scene->OnDetach();
                delete previousEntry.m_Scene;

                previousEntry.m_Scene = nullptr;
            }
        }

        m_ActiveSceneName = name;
        SceneEntry& activeEntry = m_SceneEntries[m_ActiveSceneName];

        // if it doesn't exist, create it
        if (activeEntry.m_Scene == nullptr) 
        {
            activeEntry.m_Scene = activeEntry.m_SceneFactory();
            activeEntry.m_Scene->OnAttach();
        }

        // if it wasn't yet lazy loaded, lazy load it
        if (!activeEntry.m_Scene->WasLazyLoaded()) 
        {
            activeEntry.m_Scene->OnLazyAttach();
            activeEntry.m_Scene->MarkAsLazyLoaded();
        }

        m_SceneEntries[m_ActiveSceneName].m_Scene->OnResume();

        Payload notice(PayloadType::SceneChanged, &m_ActiveSceneName);
        NotifyWith(notice);
    }

    Scene* SceneManager::GetActiveScene() 
    {
        ZPG_PROFILE_FUNCTION();
        ZPG_CORE_ASSERT(Exists(m_ActiveSceneName), "There is no active scene with the name: '{}'", m_ActiveSceneName);
        return m_SceneEntries[m_ActiveSceneName].m_Scene;
    }

}
