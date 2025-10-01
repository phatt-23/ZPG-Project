//
// Created by phatt on 9/22/25.
//

#include "ZPGine.h"

#include <imgui.h>
#include "FirstScene/FirstScene.h"
#include "SecondScene/SecondScene.h"

class ClientApp : public ZPG::Application {
public:
    ClientApp() {
        m_SceneManager.AddScene("Second", ZPG::CreateRef<SecondScene>());
        m_SceneManager.AddScene("First", ZPG::CreateRef<FirstScene>());
    }

    void OnImGuiRender() override {
        ImGui::Begin("Scene Switcher");
            for (auto& [sceneName, sceneInstance] : m_SceneManager) {
                if (ImGui::Button(sceneName.c_str())) {
                    m_SceneManager.SetActiveScene(sceneName);
                }
            }
        ImGui::End();
    }
private:
};

ZPG::Application* ZPG::CreateApplication() {
    return new ClientApp();
}

#include "Core/EntryPoint.h"    // main function gets put here

