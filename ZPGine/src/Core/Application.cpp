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

namespace ZPG{

Application::Application() {
    ZPG_CORE_ASSERT(s_Instance == nullptr, "Application already instantiated.");
    s_Instance = this;

    m_Window = Window::Create();
    m_Window->SetEventCallback([this](Event& e){ this->OnEvent(e); });
    m_Window->SetVSync(false);

    Renderer::Init();
    Input::Init();
}

Application::~Application() {
    Renderer::Shutdown();
    Input::Shutdown();
}

void Application::Run() {
    while (m_Running) {
        float currentTime = glfwGetTime();
        Timestep ts = currentTime - m_LastTime;
        m_LastTime = currentTime;

        m_SceneManager.GetActiveScene()->OnUpdate(ts);

        m_Window->OnUpdate();
    }
}

void Application::OnEvent(Event& event) {
    EventDispatcher dispatcher(event);

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
