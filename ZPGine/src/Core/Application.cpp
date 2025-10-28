//
// Created by phatt on 9/22/25.
//
#include "Application.h"
#include "Debug/Asserter.h"
#include "Event/Event.h"
#include "Event/WindowEvent.h"
#include "Renderer/Renderer.h"
#include "Input.h"
#include <GLFW/glfw3.h>
#include "Resource/ResourceManager.h"
#include "Window.h"

#include "imgui.h"
#include "ImGui/ImGuiManager.h"
#include "Timestep.h"
#include "Scene/Scene.h"

namespace ZPG { 

Application::Application() {
    ZPG_CORE_ASSERT(s_Instance == nullptr, "Application already instantiated.");
    s_Instance = this;

    m_Window = Window::Create(WindowProps("ZPGine", 1280, 720, true));

    m_Window->SetEventCallback([this](Event& e){ this->OnEvent(e); });
    m_Window->SetVSync(false);

    // initialize subsystems
    Renderer::Init();
    Input::Init();
    ImGuiManager::Init(m_Window);
    ResourceManager::Init();
}

Application::~Application() {
    ResourceManager::Shutdown();
    ImGuiManager::Shutdown();
    Input::Shutdown();
    Renderer::Shutdown();
}

void Application::Run() {
    while (m_Running) {
        float currentTime = glfwGetTime();
        m_Delta = currentTime - m_LastTime;
        m_LastTime = currentTime;
        
        m_SceneManager.GetActiveScene()->OnUpdate(m_Delta);
        m_SceneManager.GetActiveScene()->OnRender(m_Delta);
        
        ImGuiManager::BeginFrame();
            m_SceneManager.GetActiveScene()->OnImGuiRender();
            this->OnImGuiRender();
        ImGuiManager::EndFrame();
        
        m_Window->OnUpdate();
    }
}

void Application::OnEvent(Event& event) {
    EventDispatcher dispatcher(event);

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
