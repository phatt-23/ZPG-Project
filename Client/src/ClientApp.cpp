//
// Created by phatt on 9/22/25.
//

#include "ZPGine.h"

#include <imgui.h>
#include <ranges>

#include "CV7/ForestScene.h"
#include "CV8/ForestScene.h"
#include "CV8/SkydomeScene.h"
#include "HyenaScene/Scene.h"
#include "RevolverScene/Scene.h"
#include "implot/implot.h"
#include "Platform/OpenGL/OpenGLTexture.h"

using namespace ZPG;

class ClientApp : public Application {
public:
    ClientApp() 
    {
        AttachScenes();

        m_SceneManager.AttachCallback([&](Payload& payload) 
        {
            if (payload.Type == PayloadType::SceneChanged) 
            {
                m_SceneManager.GetActiveScene()->GetCameraController()->OnResize(m_ViewportSize.x, m_ViewportSize.y);
            }
        });
    }

    void AttachScenes() 
    {
        m_SceneManager.AddScene("CV7 - Forest", new CV7::ForestScene());
        m_SceneManager.AddScene("Revolver Model", []{ return new RevolverScene::RevolverScene(); }, SceneLifetime::Transient);
        m_SceneManager.AddScene("Hyena Model", []{ return new HyenaScene::HyenaScene(); }, SceneLifetime::Transient);
        m_SceneManager.AddScene("CV8 - Skydome", []{ return new CV8::SkydomeScene(); }, SceneLifetime::Transient);
        m_SceneManager.AddScene("CV8 - Forest", []{ return new CV8::ForestScene(); }, SceneLifetime::Transient);

        m_SceneManager.SetActiveScene("CV7 - Forest");
    }

    void OnImGuiRender() override 
    {
        float fps = 1.0f / m_Delta.AsSeconds();
        time += m_Delta.AsSeconds();

        ImGui::Begin("Stats");
        {   
            ImGui::Text("FPS: %f\n", fps);
            ImGui::Text("Flush Per Frame      : %d", Renderer::GetStats().FlushCountPerFrame);
            ImGui::Text("Draw Calls Per Frame : %d", Renderer::GetStats().DrawCallCountPerFrame);
            ImGui::Text("Shader Groups        : %d", Renderer::GetStats().ShaderProgramGroupCount);
            ImGui::Text("Material Groups      : %d", Renderer::GetStats().MaterialGroupCount);
            ImGui::Text("VAO Groups           : %d", Renderer::GetStats().VAOGroupCount);
        }
        ImGui::End();

        ImGui::Begin("Scene Switcher");
        {
            for (const auto& sceneName : m_SceneManager | std::views::keys) {
                if (ImGui::Button(sceneName.c_str())) {
                    m_SceneManager.SetActiveScene(sceneName);
                }
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

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Scene Viewport");
        {
            auto viewportOffset = ImGui::GetCursorPos();
            m_ViewportFocused = ImGui::IsWindowFocused();
            m_ViewportHovered = ImGui::IsWindowHovered();

            ImVec2 windowSize = ImGui::GetWindowSize();
            ImVec2 minBounds = ImGui::GetWindowPos() + viewportOffset;
            ImVec2 maxBounds = minBounds + windowSize;
            m_ViewportBounds[0] = { minBounds.x, minBounds.y };
            m_ViewportBounds[1] = { maxBounds.x, maxBounds.y };

            const ref<FrameBuffer>& mainFBO = Renderer::GetDrawData().MainFBO;
            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

            auto [mouseX, mouseY] = ImGui::GetMousePos();
            mouseX -= m_ViewportBounds[0].x;
            mouseY -= m_ViewportBounds[0].y;
            mouseY = viewportPanelSize.y - mouseY;
            v2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];

            // X = 0, Y = 0 is the bottom left corner in OpenGL

            if (mouseX >= 0 && mouseY >= 0 && mouseX < viewportSize.x && mouseY < viewportSize.y) {
                mainFBO->Bind();

                glm::u8vec4 pixelColor = mainFBO->ReadPixelByte4(mouseX, mouseY, FrameBufferAttachmentType::Color, 0);
                i32 entityID = mainFBO->ReadPixelInt(mouseX, mouseY, FrameBufferAttachmentType::Color, 1);

                ImGui::Begin("Read pixel data");
                ImGui::Text("mouse: %f %f", mouseX, mouseY);
                ImGui::Text("Pixel Data (Int): %d", entityID);
                ImGui::Text("Pixel Data (Float4): %d %d %d %d", pixelColor.x, pixelColor.y, pixelColor.z, pixelColor.w);
                ImGui::End();

                if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                    m_SceneManager.GetActiveScene()->GetEntityManager().RemoveEntity(entityID);
                }

                mainFBO->Unbind();
            }

            v2 size(viewportPanelSize.x, viewportPanelSize.y);

            if (size != m_ViewportSize) {
                Renderer::OnViewportResize(size.x, size.y);
                m_ViewportSize = size;

                m_SceneManager.GetActiveScene()->GetCameraController()->OnResize(size.x, size.y);
            }

            auto it = std::ranges::find_if(mainFBO->GetColorTextureAttachments(),
                [&](const std::pair<FrameBufferAttachment, ref<Texture>>& pair) {
                    return pair.first.AttachmentType == FrameBufferAttachmentType::Color && pair.first.Index == 0; });

            auto& colorAttachment = it->second;
            ImGui::Image(colorAttachment->GetRendererID(), ImVec2(size.x, size.y), ImVec2(0, 1), ImVec2(1, 0));

        }
        ImGui::End();
        ImGui::PopStyleVar();
    }

private:
    std::vector<float> fpsDataY;
    std::vector<float> fpsDataX;
    const size_t maxSamples = 1000; // number of frames visible in plot
    float time = 0.0f;

    v2 m_ViewportSize = { 0.0, 0.0 };
    bool m_ViewportFocused = false;
    bool m_ViewportHovered = false;
    v2 m_ViewportBounds[2] = { v2(0.0), v2(0.0) };
};

ZPG::Application* ZPG::CreateApplication() {
    return new ClientApp();
}

#include "Core/EntryPoint.h"    // the main function lives here

