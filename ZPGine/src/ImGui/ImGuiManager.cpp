#include "ImGuiManager.h"

#include "Platform/OpenGL/OpenGLGLFWImGuiManager.h"
#include "Core/Window.h"
#include "Debug/Asserter.h"
#include "Profiling/Instrumentor.h"
#include "Renderer/RendererAPI.h"

namespace ZPG {

void ImGuiManager::Init(const scope<Window>& window)
{
    ZPG_PROFILE_FUNCTION();
    ZPG_CORE_ASSERT(s_Instance == nullptr, "ImGuiManager already initialized.");

    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::OpenGL:
            s_Instance = new OpenGLGLFWImGuiManager(window.get());
            return;
        case RendererAPI::None: break;
    }

    ZPG_UNREACHABLE();
}

void ImGuiManager::Shutdown()
{
    ZPG_PROFILE_FUNCTION();
    ZPG_CORE_ASSERT(s_Instance != nullptr, "ImGuiManager isn't initialized.");
    delete s_Instance;
}

}
