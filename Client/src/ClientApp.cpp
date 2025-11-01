//
// Created by phatt on 9/22/25.
//

#include "ZPGine.h"

#include <imgui.h>
#include <ranges>

#include "CV7/ForestScene.h"
#include "HyenaScene/Scene.h"
#include "MustangScene/Scene.h"
#include "ColtM4CarbineScene/Scene.h"
#include "RevolverScene/Scene.h"
#include "implot/implot.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "spdlog/fmt/bundled/color.h"

using namespace ZPG;

class ClientApp : public Application {
public:
    ClientApp() {
        AttachScenes();
    }

    void AttachScenes() {
        m_SceneManager.AddScene("Revolver Model", []{ return new RevolverScene::RevolverScene(); }, SceneLifetime::Transient);
        m_SceneManager.AddScene("Hyena Model", []{ return new HyenaScene::HyenaScene(); }, SceneLifetime::Transient);
        m_SceneManager.AddScene("CV7 - Forest", []{ return new CV7::ForestScene(); }, SceneLifetime::Persistent);
                               
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


        ImGui::Begin("G-Buffer");
        {
            v2 size = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

            auto colorTexAttachments = Renderer::GetDrawData().GBuffer->GetTextureAttachments();

            ImGui::Text("Color RenderAttachment Count (TEX): %ld", colorTexAttachments.size());

            for (auto& [attachment, texture] : colorTexAttachments)
            {
                f32 aspect = (f32)texture->GetWidth() / (f32)texture->GetHeight();
                f32 aspectI = 1.0f / aspect;

                auto* glTexture = (OpenGLTexture*)texture.get();

                ImGui::Text("%s", texture->GetName().c_str());

                ImGui::Image(
                    glTexture->m_RendererID,
                    // if width is larger than height
                    ImVec2(size.x, size.x * aspectI),
                    ImVec2(0, 1),
                    ImVec2(1, 0)
                );

            }
        }
        ImGui::End();

        ImGui::Begin("Main Framebuffer");
        {
            v2 size = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

            auto colorTexAttachments = Renderer::GetDrawData().MainFBO->GetTextureAttachments();

            ImGui::Text("Color RenderAttachment Count (TEX): %ld", colorTexAttachments.size());

            for (auto& [attachment, texture] : colorTexAttachments)
            {
                f32 aspect = (f32)texture->GetWidth() / (f32)texture->GetHeight();
                f32 aspectI = 1.0f / aspect;

                auto* glTexture = (OpenGLTexture*)texture.get();

                ImGui::Text("%s", texture->GetName().c_str());

                ImGui::Image(
                    glTexture->m_RendererID,
                    // if width is larger than height
                    ImVec2(size.x, size.x * aspectI),
                    ImVec2(0, 1),
                    ImVec2(1, 0)
                );

            }
        }
        ImGui::End();

        ImGui::Begin("Scene Viewport");
        {
            const ref<FrameBuffer>& mainFBO = Renderer::GetDrawData().MainFBO;
            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

            v2 size(viewportPanelSize.x, viewportPanelSize.y);

            if (size != m_ViewportSize) {
                Renderer::OnViewportResize(size.x, size.y);
                m_ViewportSize = size;

                m_SceneManager.GetActiveScene()->GetCameraController()->OnResize(size.x, size.y);
            }

            const std::unordered_map<RenderAttachment, ref<Texture>>& colorTextures = mainFBO->GetColorTextureAttachments();
            const ref<Texture>& colorTexture = colorTextures.begin()->second;

            ImGui::Image(colorTexture->GetRendererID(), ImVec2(size.x, size.y), ImVec2(0, 1), ImVec2(1, 0));
        }
        ImGui::End();
    }

private:
    std::vector<float> fpsDataY;
    std::vector<float> fpsDataX;
    const size_t maxSamples = 1000; // number of frames visible in plot
    float time = 0.0f;
    v2 m_ViewportSize = { 0.0, 0.0 };

};

ZPG::Application* ZPG::CreateApplication() {
    return new ClientApp();
}

#include "Core/EntryPoint.h"    // the main function lives here

