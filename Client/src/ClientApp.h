//
// Created by phatt on 11/14/25.
//

#ifndef WORKSPACE_CLIENT_APP_H
#define WORKSPACE_CLIENT_APP_H

#include "Core/Application.h"

class ClientApp : public ZPG::Application 
{
public:
    ClientApp();

    void AttachScenes();
    void OnImGuiRender() override;

    void ShowPlots();
    void ShowMainMaps();
    void ShowGeometryBufferMaps();
    void ShowDirectionalShadowMap();
    void ShowSpotShadowFramebuffer();
    void ShowPointShadowFramebuffer();
    void ShowSceneViewportInfo();
    void ShowCameraInfo();
    void ShowProcessingInfo();

private:
    std::vector<float> fpsDataY;
    std::vector<float> fpsDataX;
    const size_t maxSamples = 1000; // number of frames visible in plot
    float time = 0.0f;

    bool m_ViewportFocused = false;
    bool m_ViewportHovered = false;
};

inline ZPG::Application* ZPG::CreateApplication() {
    return new ClientApp();
}

#include "Core/EntryPoint.h"    // the main function lives here

#endif
