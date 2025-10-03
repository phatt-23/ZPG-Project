//
// Created by phatt on 9/22/25.
//
#include "Application.h"
#include "Debug/Asserter.h"
#include "Event/Event.h"
#include "Event/WindowEvent.h"
#include "Renderer/Renderer.h"
#include "Input.h"
#include "Renderer/RenderCommand.h"
#include <GLFW/glfw3.h> 
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace ZPG { 

Application::Application() {
    ZPG_CORE_ASSERT(s_Instance == nullptr, "Application already instantiated.");
    s_Instance = this;

    m_Window = Window::Create();
    m_Window->SetEventCallback([this](Event& e){ this->OnEvent(e); });
    m_Window->SetVSync(false);

    Renderer::Init();
    Input::Init();
    ImGuiManager::Init(m_Window);
}

Application::~Application() {
    ImGuiManager::Shutdown();
    Renderer::Shutdown();
    Input::Shutdown();
}

void Application::Run() {
    while (m_Running) {
        float currentTime = glfwGetTime();
        Timestep ts = currentTime - m_LastTime;
        m_LastTime = currentTime;
        
        m_SceneManager.GetActiveScene()->OnUpdate(ts);

        m_SceneManager.GetActiveScene()->OnRender(ts);
        
        ImGuiManager::BeginFrame();
        m_SceneManager.GetActiveScene()->OnImGuiRender();
        this->OnImGuiRender();
        ImGuiManager::EndFrame();
        
        m_Window->OnUpdate();
    }
}

void Application::OnEvent(Event& event) {
    EventDispatcher dispatcher(event);

    // TODO: Put this shit into ImGuiManager or something
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    // io.WantCaptureKeyboard;
    // io.WantCaptureMouse;
    if (event.IsInCategory(EventCategoryMouse) && ImGui::GetIO().WantCaptureMouse && !Input::IsCursorGrabbed()) {
        return;
    }

    dispatcher.Dispatch<WindowCloseEvent>(ZPG_FORWARD_EVENT_TO_MEMBER_FN(Application::OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(ZPG_FORWARD_EVENT_TO_MEMBER_FN(Application::OnWindowResize));

    m_SceneManager.GetActiveScene()->OnEvent(event);
}

bool Application::OnWindowClose(WindowCloseEvent& event) {
    m_Running = false;
    return true;
}
bool Application::OnWindowResize(WindowResizeEvent& event) {
    Renderer::OnWindowResize(event.GetWidth(), event.GetHeight());
    return false;
}

}
