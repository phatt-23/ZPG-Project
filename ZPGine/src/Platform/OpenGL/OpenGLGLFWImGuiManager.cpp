#include "OpenGLGLFWImGuiManager.h"
#include "Core/Window.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "implot/implot.h"
#include "Profiling/Instrumentor.h"

namespace ZPG {

    
OpenGLGLFWImGuiManager::OpenGLGLFWImGuiManager(const Window* window) 
: m_WindowHandle((GLFWwindow*)window->GetNativeWindow()) 
{
    ZPG_PROFILE_FUNCTION();
    AttachImGui();
}

OpenGLGLFWImGuiManager::~OpenGLGLFWImGuiManager() {
    ZPG_PROFILE_FUNCTION();
}

void OpenGLGLFWImGuiManager::AttachImGui() {
    ZPG_PROFILE_FUNCTION();
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImPlot::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    // ImGui::StyleColorsLight();
    // ImGui::StyleColorsClassic();
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    const char* glsl_version = "#version 330";

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_WindowHandle, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void OpenGLGLFWImGuiManager::DetachImGui() {
    ZPG_PROFILE_FUNCTION();
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
}

void OpenGLGLFWImGuiManager::BeginFrameImpl() {
    ZPG_PROFILE_FUNCTION();
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Create a dockspace in main viewport, where central node is transparent.
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
}

void OpenGLGLFWImGuiManager::EndFrameImpl() {
    ZPG_PROFILE_FUNCTION();

    ImGuiIO& io = ImGui::GetIO();
    i32 width, height;
    glfwGetWindowSize(m_WindowHandle, &width, &height);
    io.DisplaySize = ImVec2((f32)width, (f32)height);

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(m_WindowHandle);
    }
}

void OpenGLGLFWImGuiManager::BlockEventsImpl(bool block) {
    ZPG_PROFILE_FUNCTION();
    m_BlockedEvents = block;
}

}
