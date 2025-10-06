//
// Created by phatt on 9/22/25.
//

#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Scene.h"
#include "Core.h"
#include <string>
#include <map>

namespace ZPG {

class SceneManager {
public:
    void AddScene(const std::string& name, const Ref<Scene>& scene);
    bool RemoveScene(const std::string& name);
    bool Exists(const std::string& name) const;

    void SetActiveScene(const std::string& name);
    Ref<Scene>& GetActiveScene();

    inline std::map<std::string, Ref<Scene>>::iterator begin() { return m_Scenes.begin(); }
    inline std::map<std::string, Ref<Scene>>::iterator end() { return m_Scenes.end(); }
private:
    std::map<std::string, Ref<Scene>> m_Scenes;
    std::string m_ActiveSceneName;
};

}

#endif //SCENEMANAGER_H
