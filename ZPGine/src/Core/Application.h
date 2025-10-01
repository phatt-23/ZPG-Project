//
// Created by phatt on 9/22/25.
//

#ifndef APPLICATION_H
#define APPLICATION_H

#include "Event/Event.h"
#include "Core.h"
#include "Window.h"
#include "SceneManager.h"
#include "Event/WindowEvent.h"
#include "ImGuiManager.h"

namespace ZPG {

class Application
{
public:
    Application();  // client apps make their constructor which acts as on create
    virtual ~Application();
    static Application& Get() { return *s_Instance; }

    void Run();
    void OnEvent(Event& e);
    virtual void OnImGuiRender() {};

    const Window& GetWindow() { return *m_Window; } 
private:
    bool OnWindowClose(WindowCloseEvent& event);
    bool OnWindowResize(WindowResizeEvent& event);
protected:
    static inline Application* s_Instance = nullptr;
    Scope<Window> m_Window;
    bool m_Running = true;
    SceneManager m_SceneManager;
    float m_LastTime;
};

// defined by client
Application* CreateApplication();

}

#endif //APPLICATION_H
