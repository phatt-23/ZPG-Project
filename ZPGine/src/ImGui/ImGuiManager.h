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

private:
    virtual void BeginFrameImpl() = 0;
    virtual void EndFrameImpl() = 0;

    inline static ImGuiManager* s_Instance = nullptr;
};

}