#pragma once

#include "Core/ImGuiManager.h"
#include <GLFW/glfw3.h>

namespace ZPG {

class OpenGLGLFWImGuiManager : public ImGuiManager {
public:
    OpenGLGLFWImGuiManager(const Window* window);
    ~OpenGLGLFWImGuiManager() override;

    virtual void BeginFrameImpl() override;
    virtual void EndFrameImpl() override;
private:
    void AttachImGui();
    void DetachImGui();

private:
    GLFWwindow* m_WindowHandle = nullptr;
};

}