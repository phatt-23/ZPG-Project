//
// Created by phatt on 9/22/25.
//

#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Scene.h"
#include "Core/Core.h"
#include <string>
#include <map>

namespace ZPG {

class SceneManager {
public:
    SceneManager();
    ~SceneManager();

    // By adding a scene ptr, the scene manager takes responsibility for it.
    // No other subject should ever free the scene object.
    void AddScene(const std::string& name, Scene* scene);

    // This will call delete on the removed scene.
    bool RemoveScene(const std::string& name);
    bool Exists(const std::string& name) const;

    void SetActiveScene(const std::string& name);
    Scene* GetActiveScene();

    inline std::unordered_map<std::string, Scene*>::iterator begin() { return m_Scenes.begin(); }
    inline std::unordered_map<std::string, Scene*>::iterator end() { return m_Scenes.end(); }
private:
    std::unordered_map<std::string, Scene*> m_Scenes;
    std::string m_ActiveSceneName;
};

}

#endif //SCENEMANAGER_H
