//
// Created by phatt on 11/14/25.
//

#ifndef WORKSPACE_CLIENT_APP_H
#define WORKSPACE_CLIENT_APP_H

#include "Core/Application.h"

class ClientApp : public ZPG::Application {
public:
    ClientApp();

    void AttachScenes();

    void OnImGuiRender() override;

    void ShowPlots();
    void ShowMainFramebuffer();
    void ShowGBuffer();
    void ShowDirectionalShadowFramebuffer();
    void ShowSpotShadowFramebuffer();
    void ShowPointShadowFramebuffer();
    void ShowSceneViewport();
    void ShowCameraInfo();

private:
    std::vector<float> fpsDataY;
    std::vector<float> fpsDataX;
    const size_t maxSamples = 1000; // number of frames visible in plot
    float time = 0.0f;

    ZPG::v2 m_ViewportSize = { 0.0, 0.0 };
    bool m_ViewportFocused = false;
    bool m_ViewportHovered = false;
    ZPG::v2 m_ViewportBounds[2] = { ZPG::v2(0.0), ZPG::v2(0.0) };
};

inline ZPG::Application* ZPG::CreateApplication() {
    return new ClientApp();
}

#include "Core/EntryPoint.h"    // the main function lives here

#endif