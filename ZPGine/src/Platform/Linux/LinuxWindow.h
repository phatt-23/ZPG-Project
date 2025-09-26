//
// Created by phatt on 9/22/25.
//

#ifndef LINUXWINDOW_H
#define LINUXWINDOW_H

#include "Core/Window.h"
#include <GLFW/glfw3.h>
#include "Renderer/GraphicsContext.h"

namespace ZPG {

class LinuxWindow : public Window {
public:
    LinuxWindow(const WindowProps& props);
    virtual ~LinuxWindow() override;

    virtual void OnUpdate() override;
    virtual void SetEventCallback(EventCallbackFn&& fn) override;
    virtual u32 GetWidth() const override;
    virtual u32 GetHeight() const override;
    virtual void SetVSync(bool value) override;
    virtual bool IsVSync() const override;

    virtual void* GetNativeWindow() const override;
private:
    void Init();
    void SetupCallbacks();
    void Shutdown();
private:
    GLFWwindow* m_Window = nullptr;
    GraphicsContext* m_Context = nullptr;

    struct WindowData {
        WindowProps Props;
        EventCallbackFn EventCallback = nullptr;
    } m_Data;
};

}

#endif //LINUXWINDOW_H
