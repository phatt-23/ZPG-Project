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
#include "implot/implot.h"
#include "Platform/OpenGL/OpenGLTexture2D.h"
#include "Renderer/Renderer.h"
#include "Texture/Texture2DArray.h"
#include "Texture/TextureCubeMapArray.h"
#include "Renderer/DrawCommand.h"
#include "CV10/MovementScene.h"
#include "CV10/HomoScene.h"
#include "CV10/WhackScene.h"
#include "CV10/PlanetScene.h"


using namespace ZPG;


ClientApp::ClientApp()
{
    AttachScenes();

    m_SceneManager.AttachCallback([&](Payload& payload)
    {
        if (payload.Type == PayloadType::SceneChanged)
        {
            m_SceneManager.GetActiveScene()->GetCameraController()->OnResize(m_SceneViewportInfo.Size.x, m_SceneViewportInfo.Size.y);
        }
    });
}

void ClientApp::AttachScenes()
{
    m_SceneManager.AddScene("10-Movement",  []{ return new CV10::MovementScene(); });
    m_SceneManager.AddScene("10-Homo",      []{ return new CV10::HomoScene(); });
    m_SceneManager.AddScene("10-Whack",     []{ return new CV10::WhackScene(); });
    m_SceneManager.AddScene("10-Planets",   []{ return new CV10::PlanetScene(); });
    m_SceneManager.AddScene("8-Shadow",     []{ return new CV8::ShadowScene(); });
    m_SceneManager.AddScene("8-Skydome",    []{ return new CV8::SkydomeScene(); });
    m_SceneManager.AddScene("8-Forest",     []{ return new CV8::ForestScene(); });
    m_SceneManager.SetActiveScene("10-Planets");
}

void ClientApp::OnImGuiRender()
{
    Application::OnImGuiRender();

    float fps = 1.0f / m_Delta.AsSeconds();

    ImGui::Begin("Stats");
    ImGui::Text("FPS: %f\n", fps);
    ImGui::Text("Submissions                : %d", Renderer::GetStats().Submissions);
    ImGui::SeparatorText("Batches");
    ImGui::Text("Static GeometryBuffer      : %ld", Renderer::GetRenderContext().StaticBatches.GeometryBuffer.size());
    ImGui::Text("Static Shadow              : %ld", Renderer::GetRenderContext().StaticBatches.Shadow.size());
    ImGui::Text("Static ForwardOpaque       : %ld", Renderer::GetRenderContext().StaticBatches.ForwardOpaque.size());
    ImGui::Text("Static ForwardTransparent  : %ld", Renderer::GetRenderContext().StaticBatches.ForwardTransparent.size());
    ImGui::Separator();
    ImGui::Text("Dynamic GeometryBuffer     : %ld", Renderer::GetRenderContext().Batches.GeometryBuffer.size());
    ImGui::Text("Dynamic Shadow             : %ld", Renderer::GetRenderContext().Batches.Shadow.size());
    ImGui::Text("Dynamic ForwardOpaque      : %ld", Renderer::GetRenderContext().Batches.ForwardOpaque.size());
    ImGui::Text("Dynamic ForwardTransparent : %ld", Renderer::GetRenderContext().Batches.ForwardTransparent.size());
    ImGui::SeparatorText("Queues");
    ImGui::Text("Static GeometryBuffer      : %ld", Renderer::GetRenderContext().StaticQueues.GeometryBuffer.size());
    ImGui::Text("Static Shadow              : %ld", Renderer::GetRenderContext().StaticQueues.Shadow.size());
    ImGui::Text("Static ForwardOpaque       : %ld", Renderer::GetRenderContext().StaticQueues.ForwardOpaque.size());
    ImGui::Text("Static ForwardTransparent  : %ld", Renderer::GetRenderContext().StaticQueues.ForwardTransparent.size());
    ImGui::Separator();
    ImGui::Text("Dynamic GeometryBuffer     : %ld", Renderer::GetRenderContext().Queues.GeometryBuffer.size());
    ImGui::Text("Dynamic Shadow             : %ld", Renderer::GetRenderContext().Queues.Shadow.size());
    ImGui::Text("Dynamic ForwardOpaque      : %ld", Renderer::GetRenderContext().Queues.ForwardOpaque.size());
    ImGui::Text("Dynamic ForwardTransparent : %ld", Renderer::GetRenderContext().Queues.ForwardTransparent.size());
    ImGui::End();

    ImGui::Begin("Scene Switcher");
    for (const auto& sceneName : m_SceneManager | std::views::keys) {
        if (ImGui::Button(sceneName.c_str())) {
            m_SceneManager.SetActiveScene(sceneName);
        }
    }
    ImGui::End();

    ShowPlots();
    ShowSceneViewportInfo();
    ShowMainMaps();
    ShowGeometryBufferMaps();
    ShowDirectionalShadowMap();
    ShowSpotShadowFramebuffer();
    // ShowPointShadowFramebuffer();
    ShowCameraInfo();
}

void ClientApp::ShowPlots()
{
    float fps = 1.0f / m_Delta.AsSeconds();
    time += m_Delta.AsSeconds();

    // Append a new sample
    fpsDataY.push_back(fps);
    fpsDataX.push_back(time);

    // Keep only last maxSamples elements
    if (fpsDataY.size() > maxSamples)
    {
        fpsDataY.erase(fpsDataY.begin(), fpsDataY.end() - maxSamples);
        fpsDataX.erase(fpsDataX.begin(), fpsDataX.end() - maxSamples);
    }

    ImGui::Begin("Plots");
    if (ImPlot::BeginPlot("Performance"))
    {
        // Keep the visible window scrolling forward
        float xMin = fpsDataX.front();
        float xMax = fpsDataX.back();

        // adjust X and Y range
        ImPlot::SetupAxesLimits(xMin, xMax, 0.0f, 200.0f, ImGuiCond_Always);
        ImPlot::PlotLine("FPS", fpsDataX.data(), fpsDataY.data(), fpsDataY.size());
        ImPlot::EndPlot();
    }
    ImGui::End();
}

void ClientApp::ShowMainMaps()
{
    ImGui::Begin("Main Framebuffer");

    v2 size = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };
    auto& renderContext = Renderer::GetRenderContext();

    vec attachments =
    {
        renderContext.Targets.MainColorMap,
        renderContext.Targets.MainDepthMap,
        renderContext.Targets.MainEntityIDMap,
    };

    for (auto& texture : attachments)
    {
        ImGui::Text("%s", texture->GetName().c_str());
        f32 aspect = (f32)texture->GetWidth() / (f32)texture->GetHeight();
        ImGui::Image(texture->GetRendererID(), ImVec2(size.x, size.x * 1.0f / aspect), ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();
}

void ClientApp::ShowGeometryBufferMaps()
{
    ImGui::Begin("G-Buffer");

    v2 size = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };
    auto& renderContext = Renderer::GetRenderContext();

    vec attachments =
    {
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
        ImGui::Text("%s", texture->GetName().c_str());
        f32 aspect = (f32)texture->GetWidth() / (f32)texture->GetHeight();
        ImGui::Image(texture->GetRendererID(), ImVec2(size.x, size.x * 1.0/aspect), ImVec2(0, 1), ImVec2(1, 0));
    }

    ImGui::End();
}

void ClientApp::ShowDirectionalShadowMap()
{
    ImGui::Begin("Directional Light Framebuffer");

    v2 size = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };
    auto texture = Renderer::GetRenderContext().Targets.DirectionalLightShadowMapArray;
    f32 aspect = (f32)texture->GetWidth() / (f32)texture->GetHeight();

    ImGui::Text("%s", texture->GetName().c_str());
    ImGui::Image(texture->GetRendererID(), ImVec2(size.x, size.x * 1.0/aspect), ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
}

void ClientApp::ShowSpotShadowFramebuffer()
{
    ImGui::Begin("SpotLight Framebuffer");

    auto texture = Renderer::GetRenderContext().Targets.SpotLightShadowMapArray;
    v2 size = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };
    f32 aspect = (f32)texture->GetWidth() / (f32)texture->GetHeight();

    ImGui::Text("%s", texture->GetName().c_str());
    ImGui::Image(texture->GetRendererID(), ImVec2(size.x, size.x * 1.0/aspect), ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
}

void ClientApp::ShowPointShadowFramebuffer()
{
    ImGui::Begin("PointLight Framebuffer");

    auto texture = Renderer::GetRenderContext().Targets.PointLightShadowCubeMapArray;
    v2 size = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };
    f32 aspect = (f32)texture->GetWidth() / (f32)texture->GetHeight();

    ImGui::Text("%s", texture->GetName().c_str());
    ImGui::Image(texture->GetRendererID(), ImVec2(size.x, size.x * 1.0/aspect), ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
}

void ClientApp::ShowSceneViewportInfo()
{
    ImGui::Begin("Scene Viewport Info");
        ImGui::Text("Abs Position   : %f %f", m_SceneViewportInfo.AbsPosition.x, m_SceneViewportInfo.AbsPosition.y);
        ImGui::Text("Abs Min Bounds : %f %f", m_SceneViewportInfo.AbsMinBounds.x, m_SceneViewportInfo.AbsMinBounds.y);
        ImGui::Text("Abs Max Bounds : %f %f", m_SceneViewportInfo.AbsMaxBounds.x, m_SceneViewportInfo.AbsMaxBounds.y);
        ImGui::Text("Size           : %f %f", m_SceneViewportInfo.Size.x, m_SceneViewportInfo.Size.y);
        ImGui::Text("Offset         : %f %f", m_SceneViewportInfo.Offset.x, m_SceneViewportInfo.Offset.y);
        ImGui::Text("Panel Size     : %f %f", m_SceneViewportInfo.PanelSize.x, m_SceneViewportInfo.PanelSize.y);
        ImGui::Text("Is Focused     : %d", m_SceneViewportInfo.IsFocused);
        ImGui::Text("Is Hovered     : %d", m_SceneViewportInfo.IsHovered);
        ImGui::Text("Mouse Position : %f %f", m_SceneViewportInfo.MousePosition.x, m_SceneViewportInfo.MousePosition.y);
        if (m_SceneViewportInfo.IsHovered)
        {
            auto& mainFrameBuffer = Renderer::GetRenderContext().Targets.MainFrameBuffer;
            v2 mouse = m_SceneViewportInfo.MousePosition;

            auto pixelColor = mainFrameBuffer->ReadPixelByte4(mouse.x, mouse.y, FrameBufferAttachmentType::Color, 0);
            i32 entityID = mainFrameBuffer->ReadPixelInt(mouse.x, mouse.y, FrameBufferAttachmentType::Color, 1);
            f32 depth = mainFrameBuffer->ReadPixelFloat(mouse.x, mouse.y, FrameBufferAttachmentType::Depth, 0);

            ImGui::Separator();
            ImGui::Text("Pixel Data - color     (main 0) : %d %d %d %d", pixelColor.x, pixelColor.y, pixelColor.z, pixelColor.w);
            ImGui::Text("Pixel Data - entity id (main 1) : %d", entityID);
            ImGui::Text("Pixel Data - depth     (main 2) : %f", depth);
        }
    ImGui::End();

}

void ClientApp::ShowCameraInfo()
{
    ImGui::Begin("Camera");

    auto& camera = m_SceneManager.GetActiveScene()->GetCamera();
    f32 fov = camera.GetFOV();

    if (ImGui::DragFloat("CameraFOV", &fov, 0.5, 0.0, 180.0)) {
        camera.SetFOV(fov);
        camera.CalcPerspectiveProjection();
    }

    for (auto& corner : Renderer::GetRenderContext().ViewingFrustum.GetCorners())
    {
        ImGui::Text("Corner x: %f y: %f z: %f", corner.x, corner.y, corner.z);
    }

    ImGui::End();
}
