#pragma once

namespace ZPG {

class Window;

class ImGuiManager {
public:
    virtual ~ImGuiManager() {}

    static void Init(const scope<Window>& window);
    static void Shutdown();
    static void BeginFrame() { s_Instance->BeginFrameImpl(); }
    static void EndFrame() { s_Instance->EndFrameImpl(); }
    static void BlockEvents(bool block) { s_Instance->BlockEventsImpl(block); }

private:
    virtual void BeginFrameImpl() = 0;
    virtual void EndFrameImpl() = 0;
    virtual void BlockEventsImpl(bool block) = 0;

    inline static ImGuiManager* s_Instance = nullptr;
};

}