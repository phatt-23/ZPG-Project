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
#include "implot/implot.h"

using namespace ZPG;

class ClientApp : public Application {
public:
    ClientApp() {
        AttachScenes();
    }

    void AttachScenes() {
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
        float fps = 1.0f / m_Delta.AsSeconds();
        time += m_Delta.AsSeconds();

        ImGui::Begin("Stats");
            ImGui::Text("FPS: %f\n", fps);

            static bool instancedEnabled = Renderer::IsInstanced();
            if (ImGui::Checkbox("Renderer is Instanced", &instancedEnabled)) {
                Renderer::SetInstanced(instancedEnabled);
            }

            static bool deferredEnabled = Renderer::IsDeferred();
            if (ImGui::Checkbox("Renderer is Deferred", &deferredEnabled)) {
                Renderer::SetDeferred(deferredEnabled);
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


        // Append new sample
        fpsDataY.push_back(fps);
        fpsDataX.push_back(time);

        // Keep only last maxSamples elements
        if (fpsDataY.size() > maxSamples) {
            fpsDataY.erase(fpsDataY.begin(), fpsDataY.end() - maxSamples);
            fpsDataX.erase(fpsDataX.begin(), fpsDataX.end() - maxSamples);
        }

        ImGui::Begin("Plots");
        if (ImPlot::BeginPlot("Performance")) {
            // Keep visible window scrolling forward
            float x_min = fpsDataX.front();
            float x_max = fpsDataX.back();

            // adjust X and Y range 
            ImPlot::SetupAxesLimits(x_min, x_max, 0.0f, 200.0f, ImGuiCond_Always); 

            ImPlot::PlotLine("FPS", fpsDataX.data(), fpsDataY.data(), fpsDataY.size());
            ImPlot::EndPlot();
        }
        ImGui::End();

    }

private:
    std::vector<float> fpsDataY;
    std::vector<float> fpsDataX;
    const size_t maxSamples = 1000; // number of frames visible in plot
    float time = 0.0f;

};

ZPG::Application* ZPG::CreateApplication() {
    return new ClientApp();
}

#include "Core/EntryPoint.h"    // the main function lives here

