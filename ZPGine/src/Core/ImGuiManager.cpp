#include "ImGuiManager.h"

#include "Application.h"
#include "Renderer/Renderer.h"
#include <GLFW/glfw3.h>
#include "Platform/OpenGL/OpenGLGLFWImGuiManager.h"

namespace ZPG {

void ImGuiManager::Init(const Scope<Window>& window) {
    ZPG_CORE_ASSERT(s_Instance == nullptr, "ImGuiManager already initialized.");

    switch (Renderer::GetAPI()) {
    case RendererAPI::OpenGL: 
        s_Instance = new OpenGLGLFWImGuiManager(window.get());
        return;
    case RendererAPI::None: break;
    }
    ZPG_UNREACHABLE();
}
void ImGuiManager::Shutdown() {
    ZPG_CORE_ASSERT(s_Instance != nullptr, "ImGuiManager isn't initialized.");
    free(s_Instance);
}


}
