//
// Created by phatt on 9/22/25.
//

#ifndef APPLICATION_H
#define APPLICATION_H

#include "Core/Timestep.h"
#include "Scene/SceneManager.h"

namespace ZPG {

class WindowCloseEvent;
class WindowResizeEvent;
class Window;
class Event;

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
    scope<Window> m_Window;
    SceneManager m_SceneManager;  // fully owned by the application

    bool m_Running = true;
    float m_LastTime = 0;
    Timestep m_Delta = 0;

    v2 m_ViewportSize = {0, 0};
private:
    static inline Application* s_Instance = nullptr;
};

// defined by client
Application* CreateApplication();

}

#endif //APPLICATION_H
