//
// Created by phatt on 9/22/25.
//

#include "ZPGine.h"

#include <imgui.h>
#include <ranges>

#include "PBRSpheresScene/SpheresScene.h"
#include "CV5/SpheresScene.h"
#include "CV5/ForestScene.h"
#include "CV5/SolarSystemScene.h"
#include "CV6/ForestScene.h"
#include "CV6/SolarSystemScene.h"
#include "CV6/SpheresScene.h"

class ClientApp : public ZPG::Application {
public:
    ClientApp() {
        using namespace ZPG;

        m_SceneManager.AddScene("CV5 - Spheres", new CV5::SpheresScene());
        m_SceneManager.AddScene("CV5 - Forest", new CV5::ForestScene());
        m_SceneManager.AddScene("CV5 - Solar System", new CV5::SolarSystemScene());
        m_SceneManager.AddScene("Spheres Scene", new PBRSpheresSceneNS::SpheresScene());
        m_SceneManager.AddScene("CV6 - Forest", new CV6::ForestScene());
        m_SceneManager.AddScene("CV6 - Solar System", new CV6::SolarSystemScene());
        m_SceneManager.AddScene("CV6 - Spheres", new CV6::SpheresScene());

        // m_SceneManager.SetActiveScene("Spheres Scene");
    }

    void OnImGuiRender() override {
        ImGui::Begin("Scene Switcher");
            for (const auto& sceneName : m_SceneManager | std::views::keys) {
                if (ImGui::Button(sceneName.c_str())) {
                    m_SceneManager.SetActiveScene(sceneName);
                }
            }
        ImGui::End();
    }
};

ZPG::Application* ZPG::CreateApplication() {
    return new ClientApp();
}

#include "Core/EntryPoint.h"    // the main function lives here

