//
// Created by phatt on 9/22/25.
//

#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

namespace ZPG {

class Scene;

enum class SceneLifetime {
    None = 0,
    Transient,
    Persistent,
};

class SceneManager {
public:
    struct SceneEntry {
        std::function<Scene*(void)> m_SceneFactory = nullptr;
        Scene* m_Scene = nullptr;
        SceneLifetime m_Lifetime = SceneLifetime::None;
    };

public:
    SceneManager();
    ~SceneManager();

    // By adding a scene ptr, the scene manager takes responsibility for it.
    // No other subject should ever free the scene object.
    void AddScene(const std::string& name, const std::function<Scene*(void)>& sceneFactory, SceneLifetime lifetime = SceneLifetime::Transient);
    void AddScene(const std::string& name, Scene* scene);

    // This will call delete on the removed scene.
    bool RemoveScene(const std::string& name);
    bool Exists(const std::string& name) const;

    void SetActiveScene(const std::string& name);
    Scene* GetActiveScene();

    std::unordered_map<std::string, SceneEntry>::iterator begin() { return m_SceneEntries.begin(); }
    std::unordered_map<std::string, SceneEntry>::iterator end() { return m_SceneEntries.end(); }

private:
    std::unordered_map<std::string, SceneEntry> m_SceneEntries;
    std::string m_ActiveSceneName;
};

}

#endif
