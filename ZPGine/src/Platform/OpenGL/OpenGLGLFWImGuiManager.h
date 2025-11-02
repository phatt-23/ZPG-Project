#pragma once

#include "ImGui/ImGuiManager.h"

namespace ZPG {

class OpenGLGLFWImGuiManager : public ImGuiManager {
public:
    OpenGLGLFWImGuiManager(const Window* window);
    ~OpenGLGLFWImGuiManager() override;

    virtual void BeginFrameImpl() override;
    virtual void EndFrameImpl() override;
    virtual void BlockEventsImpl(bool block) override;
private:
    void AttachImGui();
    void DetachImGui();

private:
    GLFWwindow* m_WindowHandle = nullptr;
    bool m_BlockedEvents = false;  // isn't used anywhere yet
};

}