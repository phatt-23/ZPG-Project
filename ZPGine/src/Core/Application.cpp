//
// Created by phatt on 9/22/25.
//
#include "Application.h"
#include "Debug/Asserter.h"
#include "Event/Event.h"
#include "Event/WindowEvent.h"
#include "Input.h"
#include <GLFW/glfw3.h>
#include "Resource/ResourceManager.h"
#include "Window.h"

#include "imgui.h"
#include "ImGui/ImGuiManager.h"
#include "Timestep.h"
#include "Profiling/Instrumentor.h"
#include "Scene/Scene.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderCommand.h"

namespace ZPG { 

Application::Application() {
    ZPG_PROFILE_FUNCTION();
    ZPG_CORE_ASSERT(s_Instance == nullptr, "Application already instantiated.");
    s_Instance = this;

    m_Window = Window::Create(WindowProps("ZPGine", 1280, 720, true));

    m_Window->SetEventCallback([this](Event& e)
    {
        this->OnEvent(e);
    });

    m_Window->SetVSync(false);

    // initialize subsystems
    Input::Init();
    ImGuiManager::Init(m_Window);
    ResourceManager::Init();
    RenderCommand::Init();
    Renderer::Init();
}

Application::~Application() {
    ZPG_PROFILE_FUNCTION();
    ResourceManager::Shutdown();
    ImGuiManager::Shutdown();
    Input::Shutdown();
    RenderCommand::Shutdown();
    Renderer::Shutdown();
}

void Application::Run() {
    ZPG_PROFILE_FUNCTION();
    while (m_Running) {
        float currentTime = glfwGetTime();
        m_Delta = currentTime - m_LastTime;
        m_LastTime = currentTime;

        ImGuiManager::BeginFrame();
            m_SceneManager.GetActiveScene()->OnUpdate(m_Delta);
            Renderer::RenderScene(*m_SceneManager.GetActiveScene());

            m_SceneManager.GetActiveScene()->OnImGuiRender();
            this->OnImGuiRender();

            m_Window->OnUpdate();
        ImGuiManager::EndFrame();
    }
}

void Application::OnEvent(Event& event) {
    ZPG_PROFILE_FUNCTION();
    EventDispatcher dispatcher(event);

    // if (event.IsInCategory(EventCategoryMouse) && ImGui::GetIO().WantCaptureMouse && !Input::IsCursorGrabbed()) {
    //     return;
    // }

    dispatcher.Dispatch<WindowCloseEvent>(ZPG_FORWARD_EVENT_TO_MEMBER_FN(Application::OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(ZPG_FORWARD_EVENT_TO_MEMBER_FN(Application::OnWindowResize));

    m_SceneManager.GetActiveScene()->OnEvent(event);
}

bool Application::OnWindowClose(WindowCloseEvent& event) {
    ZPG_PROFILE_FUNCTION();
    m_Running = false;
    return true;
}

bool Application::OnWindowResize(WindowResizeEvent& event) {
    ZPG_PROFILE_FUNCTION();
    // Renderer::OnWindowResize(event.GetWidth(), event.GetHeight());
    return false;
}

}
