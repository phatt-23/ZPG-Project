#include "SceneManager.h"
#include "Debug/Asserter.h"

namespace ZPG {
void SceneManager::AddScene(const std::string& name, const Ref<Scene>& scene) {
    ZPG_CORE_DEBUG("SceneManager added scene with name {}", name);
    ZPG_CORE_ASSERT(!Exists(name), "Scene with the given name already exists."); 
    m_Scenes[name] = scene;

    // set this scene as active if the current active scene name doesnt point to any scene
    if (!Exists(m_ActiveSceneName)) {
        m_ActiveSceneName = name;
    }

    scene->OnAttach();
}

bool SceneManager::RemoveScene(const std::string& name) {
    if (Exists(name)) {
        auto& layer = m_Scenes[name];
        m_Scenes.erase(name);
        layer->OnDetach();
        return true;
    }
    return false;
}

bool SceneManager::Exists(const std::string& name) const {
    return m_Scenes.contains(name);
}

void SceneManager::SetActiveScene(const std::string& name) {
    ZPG_CORE_ASSERT(Exists(name), "Cannot set scene with name '{}' as active, it doesn't exist.", name);
    m_ActiveSceneName = name; 
}

Ref<Scene>& SceneManager::GetActiveScene() {
    ZPG_CORE_ASSERT(Exists(m_ActiveSceneName), "There is no active scene with the name: '{}'", m_ActiveSceneName);
    return m_Scenes[m_ActiveSceneName];
}


}
