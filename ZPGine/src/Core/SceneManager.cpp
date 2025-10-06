#include "SceneManager.h"
#include "Debug/Asserter.h"

namespace ZPG {

SceneManager::SceneManager() {

}

SceneManager::~SceneManager() {
    for (auto& [name, scene] : m_Scenes) {
        delete scene;
    }
}

void SceneManager::AddScene(const std::string& name, Scene* scene) {
    ZPG_CORE_DEBUG("SceneManager added scene with name {}", name);
    ZPG_CORE_ASSERT(!Exists(name), "Scene with the given name already exists."); 
    m_Scenes[name] = scene;

    // set this scene as active if the current active scene name doesnt point to any scene
    if (!Exists(m_ActiveSceneName)) {
        SetActiveScene(name);
    }

    scene->OnAttach();
}

bool SceneManager::RemoveScene(const std::string& name) {
    if (Exists(name)) {
        Scene* scene = m_Scenes[name];
        m_Scenes.erase(name);
        scene->OnDetach();
        delete scene;
        return true;
    }
    return false;
}

bool SceneManager::Exists(const std::string& name) const {
    return m_Scenes.contains(name);
}

void SceneManager::SetActiveScene(const std::string& name) {
    ZPG_CORE_ASSERT(Exists(name), "Cannot set scene with name '{}' as active, it doesn't exist.", name);
    if (Exists(m_ActiveSceneName)) {
        m_Scenes[m_ActiveSceneName]->OnPause();
    }
    m_ActiveSceneName = name; 
    m_Scenes[m_ActiveSceneName]->OnResume();
}

Scene* SceneManager::GetActiveScene() {
    ZPG_CORE_ASSERT(Exists(m_ActiveSceneName), "There is no active scene with the name: '{}'", m_ActiveSceneName);
    return m_Scenes[m_ActiveSceneName];
}

}
