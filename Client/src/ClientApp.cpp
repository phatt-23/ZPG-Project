//
// Created by phatt on 9/22/25.
//

#include "ZPGine.h"

#include <imgui.h>
#include <ranges>

#include "PBRSpheresScene/SpheresScene.h"
#include "CV6/ForestScene.h"
#include "CV6/SolarSystemScene.h"
#include "CV6/SpheresScene.h"
#include "HyenaScene/Scene.h"
#include "MustangScene/Scene.h"
#include "ColtM4CarbineScene/Scene.h"
#include "RevolverScene/Scene.h"

using namespace ZPG;

class ClientApp : public Application {
public:
    ClientApp() {

        m_SceneManager.AddScene("Spheres Scene",        new PBRSpheresSceneNS::SpheresScene());
        m_SceneManager.AddScene("CV6 - Forest",         new CV6::ForestScene());
        m_SceneManager.AddScene("CV6 - Solar System",   new CV6::SolarSystemScene());
        m_SceneManager.AddScene("CV6 - Spheres",        new CV6::SpheresScene());
        m_SceneManager.AddScene("Hyena Model",          new HyenaScene::HyenaScene());
        m_SceneManager.AddScene("Mustang Model",        new MustangScene::MustangScene());
        m_SceneManager.AddScene("Colt Model",           new ColtM4CarbineScene::ColtM4CarbineScene());
        m_SceneManager.AddScene("Revolver Model",       new RevolverScene::RevolverScene());
                               
        m_SceneManager.SetActiveScene("Revolver Model");
    }

    void OnImGuiRender() override {

        ImGui::Begin("Stats");
            ImGui::Text("FPS: %f\n", 1.0f / m_Delta.AsSeconds());

            static bool instancedEnabled = Renderer::IsInstanced();
            if (ImGui::Checkbox("Renderer is Instanced", &instancedEnabled)) {
                Renderer::SetInstanced(instancedEnabled);
            }

            ImGui::Text("Flush Per Frame      : %d", Renderer::GetStats().FlushCountPerFrame);
            ImGui::Text("Draw Calls Per Frame : %d", Renderer::GetStats().DrawCallCountPerFrame);
            ImGui::Text("Shader Groups        : %d", Renderer::GetStats().ShaderProgramGroupCount);
            ImGui::Text("Material Groups      : %d", Renderer::GetStats().MaterialGroupCount);
            ImGui::Text("VAO Groups           : %d", Renderer::GetStats().VAOGroupCount);
        ImGui::End();

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

