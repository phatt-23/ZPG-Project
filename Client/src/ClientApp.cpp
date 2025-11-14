//
// Created by phatt on 9/22/25.
//

#include "ClientApp.h"

#include "ZPGine.h"

#include <imgui.h>
#include <ranges>

#include "CV7/ForestScene.h"
#include "CV8/ForestScene.h"
#include "CV8/SkydomeScene.h"
#include "CV8/ShadowScene.h"
#include "HyenaScene/Scene.h"
#include "RevolverScene/Scene.h"
#include "implot/implot.h"
#include "Platform/OpenGL/OpenGLTexture2D.h"
#include "Renderer/MultipassRenderer.h"


using namespace ZPG;


ClientApp::ClientApp() {
    AttachScenes();

    m_SceneManager.AttachCallback([&](Payload& payload)
    {
        if (payload.Type == PayloadType::SceneChanged)
        {
            m_SceneManager.GetActiveScene()->GetCameraController()->OnResize(m_ViewportSize.x, m_ViewportSize.y);
        }
    });
}

void ClientApp::AttachScenes() {
    m_SceneManager.AddScene("CV8 - Shadow", []{ return new CV8::ShadowScene(); }, SceneLifetime::Transient);
    m_SceneManager.AddScene("CV8 - Skydome", []{ return new CV8::SkydomeScene(); }, SceneLifetime::Transient);
    m_SceneManager.AddScene("CV8 - Forest", []{ return new CV8::ForestScene(); }, SceneLifetime::Transient);
    m_SceneManager.AddScene("Revolver Model", []{ return new RevolverScene::RevolverScene(); }, SceneLifetime::Transient);
    m_SceneManager.AddScene("Hyena Model", []{ return new HyenaScene::HyenaScene(); }, SceneLifetime::Transient);

    m_SceneManager.SetActiveScene("CV8 - Shadow");
}

void ClientApp::OnImGuiRender() {
    float fps = 1.0f / m_Delta.AsSeconds();
    time += m_Delta.AsSeconds();

    ImGui::Begin("Stats");
    {
        ImGui::Text("FPS: %f\n", fps);
        ImGui::Text("Flush Per Frame      : %d", MultipassRenderer::GetStats().FlushCountPerFrame);
        ImGui::Text("Draw Calls Per Frame : %d", MultipassRenderer::GetStats().DrawCallCountPerFrame);
        ImGui::Text("Shader Groups        : %d", MultipassRenderer::GetStats().ShaderProgramGroupCount);
        ImGui::Text("Material Groups      : %d", MultipassRenderer::GetStats().MaterialGroupCount);
        ImGui::Text("VAO Groups           : %d", MultipassRenderer::GetStats().VAOGroupCount);
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

    ShowPlots();
    ShowSceneViewport();
}

void ClientApp::ShowPlots() {
    float fps = 1.0f / m_Delta.AsSeconds();
    time += m_Delta.AsSeconds();

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

void ClientApp::ShowMainFramebuffer() {

    ImGui::Begin("Main Framebuffer");
    {
        v2 size = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

        auto colorTexAttachments = MultipassRenderer::GetRenderContext().MainFramebuffer->GetTextureAttachments();

        ImGui::Text("Texture Count (TEX): %ld", colorTexAttachments.size());

        for (auto& texture : colorTexAttachments | std::views::values)
        {
            f32 aspect = (f32)texture->GetWidth() / (f32)texture->GetHeight();
            f32 aspectI = 1.0f / aspect;

            ImGui::Text("%s", texture->GetName().c_str());

            ImGui::Image(
                texture->GetRendererID(),
                // if width is larger than height
                ImVec2(size.x, size.x * aspectI),
                ImVec2(0, 1),
                ImVec2(1, 0)
            );

        }
    }
    ImGui::End();
}

void ClientApp::ShowGBuffer() {
    ImGui::Begin("G-Buffer");
    {
        v2 size = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

        auto& renderContext = MultipassRenderer::GetRenderContext();
        auto& colorTexAttachments = renderContext.GeometryPassFramebuffer->GetTextureAttachments();

        ImGui::Text("Color RenderAttachment Count (TEX): %ld", colorTexAttachments.size());

        for (const auto& texture : colorTexAttachments | std::views::values)
        {
            f32 aspect = (f32)texture->GetWidth() / (f32)texture->GetHeight();
            f32 aspectI = 1.0f / aspect;

            ImGui::Text("%s", texture->GetName().c_str());

            ImGui::Image(texture->GetRendererID(),
                         // if width is larger than height
                         ImVec2(size.x, size.x * aspectI),
                         ImVec2(0, 1),
                         ImVec2(1, 0)
            );

        }
    }
    ImGui::End();
}

void ClientApp::ShowDirectionalShadowFramebuffer() {

    ImGui::Begin("Directional Light Framebuffer");
    {
        v2 size = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

        auto colorTexAttachments = MultipassRenderer::GetRenderContext().DirectionalLightShadowFramebuffer->GetTextureAttachments();

        ImGui::Text("Color RenderAttachment Count (TEX): %ld", colorTexAttachments.size());

        for (auto& texture : colorTexAttachments | std::views::values)
        {
            f32 aspect = (f32)texture->GetWidth() / (f32)texture->GetHeight();
            f32 aspectI = 1.0f / aspect;

            ImGui::Text("%s", texture->GetName().c_str());

            ImGui::Image(
                texture->GetRendererID(),
                // if width is larger than height
                ImVec2(size.x, size.x * aspectI),
                ImVec2(0, 1),
                ImVec2(1, 0)
            );

        }
    }
    ImGui::End();
}

void ClientApp::ShowSpotShadowFramebuffer() {

    ImGui::Begin("SpotLight Framebuffer");
    {
        v2 size = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

        auto colorTexAttachments = MultipassRenderer::GetRenderContext().SpotLightShadowFramebuffer->GetTextureAttachments();

        ImGui::Text("Color RenderAttachment Count (TEX): %ld", colorTexAttachments.size());

        for (auto& texture : colorTexAttachments | std::views::values)
        {
            f32 aspect = (f32)texture->GetWidth() / (f32)texture->GetHeight();
            f32 aspectI = 1.0f / aspect;

            ImGui::Text("%s", texture->GetName().c_str());

            ImGui::Image(
                texture->GetRendererID(),
                // if width is larger than height
                ImVec2(size.x, size.x * aspectI),
                ImVec2(0, 1),
                ImVec2(1, 0)
            );

        }
    }
    ImGui::End();
}

void ClientApp::ShowPointShadowFramebuffer() {

    ImGui::Begin("PointLight Framebuffer");
    {
        v2 size = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

        auto colorTexAttachments = MultipassRenderer::GetRenderContext().PointLightShadowFramebuffer->GetTextureAttachments();

        ImGui::Text("Color RenderAttachment Count (TEX): %ld", colorTexAttachments.size());

        for (auto& texture : colorTexAttachments | std::views::values)
        {
            f32 aspect = (f32)texture->GetWidth() / (f32)texture->GetHeight();
            f32 aspectI = 1.0f / aspect;

            ImGui::Text("%s", texture->GetName().c_str());

            ImGui::Image(
                texture->GetRendererID(),
                // if width is larger than height
                ImVec2(size.x, size.x * aspectI),
                ImVec2(0, 1),
                ImVec2(1, 0)
            );

        }
    }
    ImGui::End();
}

void ClientApp::ShowSceneViewport() {

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

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

        auto [mouseX, mouseY] = ImGui::GetMousePos();
        mouseX -= m_ViewportBounds[0].x;
        mouseY -= m_ViewportBounds[0].y;
        mouseY = viewportPanelSize.y - mouseY;
        v2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0]; // X = 0, Y = 0 is the bottom left corner in OpenGL

        const ref<FrameBuffer>& mainFBO = MultipassRenderer::GetRenderContext().MainFramebuffer;
        const ref<FrameBuffer>& geometryPassFramebuffer = MultipassRenderer::GetRenderContext().GeometryPassFramebuffer;
        const ref<FrameBuffer>& directionalShadowFramebuffer = MultipassRenderer::GetRenderContext().DirectionalLightShadowFramebuffer;

        if (mouseX >= 0 && mouseY >= 0 && mouseX < viewportSize.x && mouseY < viewportSize.y) {

            mainFBO->Bind();
            glm::u8vec4  pixelColor = mainFBO->ReadPixelByte4(mouseX, mouseY, FrameBufferAttachmentType::Color, 0);
            i32            entityID = mainFBO->ReadPixelInt(mouseX, mouseY, FrameBufferAttachmentType::Color, 1);
            mainFBO->Unbind();

            geometryPassFramebuffer->Bind();
            v4 pixel0 = geometryPassFramebuffer->ReadPixelFloat4(mouseX, mouseY, FrameBufferAttachmentType::Color, 0);
            v4 pixel1 = geometryPassFramebuffer->ReadPixelFloat4(mouseX, mouseY, FrameBufferAttachmentType::Color, 1);
            v4 pixel2 = geometryPassFramebuffer->ReadPixelFloat4(mouseX, mouseY, FrameBufferAttachmentType::Color, 2);
            v4 pixel3 = geometryPassFramebuffer->ReadPixelFloat4(mouseX, mouseY, FrameBufferAttachmentType::Color, 3);
            geometryPassFramebuffer->Unbind();

            directionalShadowFramebuffer->Bind();
            v4 dirShadowPixel = directionalShadowFramebuffer->ReadPixelFloat4(mouseX, mouseY, FrameBufferAttachmentType::Color, 0);
            directionalShadowFramebuffer->Unbind();

            ImGui::Begin("Read pixel data");

            ImGui::Text("mouse: %f %f", mouseX, mouseY);
            ImGui::Text("Pixel Data (Float4) (main 0): %d %d %d %d", pixelColor.x, pixelColor.y, pixelColor.z, pixelColor.w);
            ImGui::Text("Pixel Data (Int)    (main 1): %d", entityID);

            ImGui::Text("Pixel Data (Float4) (g-buffer 0): %f %f %f %f", pixel0.x, pixel0.y, pixel0.z, pixel0.w);
            ImGui::Text("Pixel Data (Float4) (g-buffer 1): %f %f %f %f", pixel1.x, pixel1.y, pixel1.z, pixel1.w);
            ImGui::Text("Pixel Data (Float4) (g-buffer 2): %f %f %f %f", pixel2.x, pixel2.y, pixel2.z, pixel2.w);
            ImGui::Text("Pixel Data (Float4) (g-buffer 3): %f %f %f %f", pixel3.x, pixel3.y, pixel3.z, pixel3.w);

            ImGui::Text("Directional Shadow Pixel Data (Float4): %f %f %f %f", dirShadowPixel.x, dirShadowPixel.y, dirShadowPixel.z, dirShadowPixel.w);

            ImGui::End();

            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                m_SceneManager.GetActiveScene()->GetEntityManager().RemoveEntity(entityID);
            }

        }

        v2 size(viewportPanelSize.x, viewportPanelSize.y);

        if (size != m_ViewportSize) {
            MultipassRenderer::OnResize(size.x, size.y);
            m_ViewportSize = size;

            m_SceneManager.GetActiveScene()->GetCameraController()->OnResize(size.x, size.y);
        }

        auto it = std::ranges::find_if(mainFBO->GetColorTextureAttachments(),
                                       [&](const std::pair<FrameBufferAttachment, ref<Texture>>& pair) {
                                           return pair.first.AttachType == FrameBufferAttachmentType::Color && pair.first.Index == 0; });

        auto& colorAttachment = it->second;
        ImGui::Image(colorAttachment->GetRendererID(), ImVec2(size.x, size.y), ImVec2(0, 1), ImVec2(1, 0));

    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void ClientApp::ShowCameraInfo() {
    ImGui::Begin("Camera");
    {
        auto& camera = m_SceneManager.GetActiveScene()->GetCamera();

        f32 fov = camera.GetFOV();
        if (ImGui::DragFloat("CameraFOV", &fov, 0.5, 0.0, 180.0)) {
            camera.SetFOV(fov);
            camera.CalcPerspectiveProjection();
        }

    }
    ImGui::End();
}
