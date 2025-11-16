//
// Created by phatt on 9/22/25.
//

#include "ClientApp.h"

#include "ZPGine.h"

#include <imgui.h>
#include <ranges>

#include "CV8/ForestScene.h"
#include "CV8/SkydomeScene.h"
#include "CV8/ShadowScene.h"
#include "HyenaScene/Scene.h"
#include "RevolverScene/Scene.h"
#include "implot/implot.h"
#include "Platform/OpenGL/OpenGLTexture2D.h"
#include "Renderer/Renderer.h"
#include "Texture/Texture2DArray.h"
#include "Texture/TextureCubeMapArray.h"


using namespace ZPG;


ClientApp::ClientApp()
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

void ClientApp::AttachScenes()
{
    m_SceneManager.AddScene("CV8 - Shadow", []{ return new CV8::ShadowScene(); }, SceneLifetime::Transient);
    m_SceneManager.AddScene("CV8 - Skydome", []{ return new CV8::SkydomeScene(); }, SceneLifetime::Transient);
    m_SceneManager.AddScene("CV8 - Forest", []{ return new CV8::ForestScene(); }, SceneLifetime::Transient);
    m_SceneManager.AddScene("Revolver Model", []{ return new RevolverScene::RevolverScene(); }, SceneLifetime::Transient);
    m_SceneManager.AddScene("Hyena Model", []{ return new HyenaScene::HyenaScene(); }, SceneLifetime::Transient);

    m_SceneManager.SetActiveScene("CV8 - Shadow");
}

void ClientApp::OnImGuiRender()
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

    ShowPlots();
    ShowSceneViewport();
    ShowMainMaps();
    ShowGeometryBufferMaps();
    ShowDirectionalShadowMap();
    // ShowSpotShadowFramebuffer();
    // ShowPointShadowFramebuffer();
    ShowCameraInfo();
}

void ClientApp::ShowPlots()
{
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

void ClientApp::ShowMainMaps()
{
    ImGui::Begin("Main Framebuffer");
    v2 size = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

    vec attachments = {
        Renderer::GetRenderContext().Targets.MainColorMap,
        Renderer::GetRenderContext().Targets.MainDepthMap,
        Renderer::GetRenderContext().Targets.MainEntityIDMap,
    };

    for (auto& texture : attachments)
    {
        f32 aspect = (f32)texture->GetWidth() / (f32)texture->GetHeight();
        f32 aspectI = 1.0f / aspect;

        ImGui::Text("%s", texture->GetName().c_str());

        ImGui::Image(
            texture->GetRendererID(),
            ImVec2(size.x, size.x * aspectI), // if width is larger than height
            ImVec2(0, 1),
            ImVec2(1, 0)
        );
    }
    ImGui::End();
}

void ClientApp::ShowGeometryBufferMaps()
{
    ImGui::Begin("G-Buffer");
    v2 size = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

    auto& renderContext = Renderer::GetRenderContext();
    vec attachments ={
        renderContext.Targets.GeometryDepthMap,
        renderContext.Targets.GeometryPositionMap,
        renderContext.Targets.GeometryNormalMap,
        renderContext.Targets.GeometryAlbedoMetallicMap,
        renderContext.Targets.GeometryEmissiveRoughnessMap,
        renderContext.Targets.GeometryEntityIDMap,
    };

    ImGui::Text("Color RenderAttachment Count (TEX): %ld", attachments.size());

    for (const auto& texture : attachments)
    {
        if (texture == nullptr)
            continue;

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
    ImGui::End();
}

void ClientApp::ShowDirectionalShadowMap()
{
    ImGui::Begin("Directional Light Framebuffer");
    v2 size = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

    if (auto texture = Renderer::GetRenderContext().Targets.DirectionalLightShadowMap) {
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
    ImGui::End();
}

void ClientApp::ShowSpotShadowFramebuffer()
{
    ImGui::Begin("SpotLight Framebuffer");
    if (auto texture = Renderer::GetRenderContext().Targets.SpotLightShadowMapArray)
    {
        v2 size = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

        f32 aspect = (f32)texture->GetWidth() / (f32)texture->GetHeight();
        f32 aspectI = 1.0f / aspect;

        ImGui::Text("%s", texture->GetName().c_str());

        ImGui::Image(
            texture->GetRendererID(),
            ImVec2(size.x, size.x * aspectI), // if width is larger than height
            ImVec2(0, 1),
            ImVec2(1, 0)
        );
    }
    ImGui::End();
}

void ClientApp::ShowPointShadowFramebuffer()
{
    ImGui::Begin("PointLight Framebuffer");
    if (auto texture = Renderer::GetRenderContext().Targets.PointLightShadowCubeMapArray)
    {
        v2 size = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

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
    ImGui::End();
}

void ClientApp::ShowSceneViewport()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Scene Viewport");
    {
        v2 size = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

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

        auto& mainFBO = Renderer::GetRenderContext().Targets.MainFrameBuffer;

        ImGui::Begin("Viewport Data");
        ImGui::Text("Content Available: %f %f", size.x, size.y);
        ImGui::Text("Viewport Size: %f %f", viewportSize.x, viewportSize.y);
        ImGui::Text("Mouse Position: %f %f", mouseX, mouseY);
        ImGui::Text("Viewport Panel Size: %f %f", viewportPanelSize.x, viewportPanelSize.y);
        ImGui::Text("Viewport Offset: %f %f", viewportOffset.x, viewportOffset.y);
        ImGui::End();

        if (mouseX >= 0 && mouseY >= 0 && mouseX < viewportSize.x && mouseY < viewportSize.y)
        {
            glm::u8vec4 pixelColor = mainFBO->ReadPixelByte4(mouseX, mouseY, FrameBufferAttachmentType::Color, 0);
            i32 entityID = mainFBO->ReadPixelInt(mouseX, mouseY, FrameBufferAttachmentType::Color, 1);
            f32 depth = mainFBO->ReadPixelFloat(mouseX, mouseY, FrameBufferAttachmentType::Depth, 0);

            ImGui::Begin("Read Pixel Data");
            ImGui::Text("MainFBO Size: %d %d", mainFBO->GetSpecification().Width, mainFBO->GetSpecification().Height);
            ImGui::Separator();
            ImGui::Text("Pixel Data (Float4) (main 0): %d %d %d %d", pixelColor.x, pixelColor.y, pixelColor.z, pixelColor.w);
            ImGui::Text("Pixel Data (Int)    (main 1): %d", entityID);
            ImGui::Text("Pixel Data (Float)  (main 2): %f", depth);
            ImGui::End();

            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
            {
                m_SceneManager.GetActiveScene()->GetEntityManager().RemoveEntity(entityID);
            }
        }

        if (size != m_ViewportSize)
        {
            m_ViewportSize = size;
            Renderer::OnResize(size.x, size.y);
            m_SceneManager.GetActiveScene()->GetCameraController()->OnResize(size.x, size.y);
        }

        ImGui::Image(
            Renderer::GetRenderContext().Targets.MainColorMap->GetRendererID(),
            ImVec2(size.x, size.y),
            ImVec2(0, 1),
            ImVec2(1, 0)
        );
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void ClientApp::ShowCameraInfo()
{
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
