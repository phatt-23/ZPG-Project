//
// Created by phatt on 9/22/25.
//

#ifndef WINDOW_H
#define WINDOW_H

#include <functional>
#include "Core.h"
#include "Event/Event.h"

namespace ZPG {

struct WindowProps {
    std::string Title;
    u32 Width, Height;
    bool VSync;

    WindowProps(const std::string& title = "ZPG Window", u32 width = 800, u32 height = 600, bool vSync = true)
        : Title(title), Width(width), Height(height), VSync(vSync)
    {}
};

class Window {
public:
    using EventCallbackFn = std::function<void(Event&)>;

    Window() = default;
    virtual ~Window() = default;

    virtual void OnUpdate() = 0;
    virtual void SetEventCallback(EventCallbackFn&& fn) = 0;
    virtual u32 GetWidth() const = 0;
    virtual u32 GetHeight() const = 0;
    virtual void SetVSync(bool value) = 0;
    virtual bool IsVSync() const = 0;

    virtual void* GetNativeWindow() const = 0;
    virtual f32 GetAspectRatio() const { return GetWidth() / (f32)GetHeight(); }

    static Scope<Window> Create(const WindowProps& props = WindowProps());
};

}

#endif //WINDOW_H
