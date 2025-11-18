//
// Created by phatt on 9/22/25.
//

#pragma once

#include "Core/Timestep.h"
#include "ImGui/ImGuiViewportInfo.h"
#include "Scene/SceneManager.h"

namespace ZPG {

    class WindowCloseEvent;
    class WindowResizeEvent;
    class Window;
    class Event;

    class Application
    {
    public:
        Application();
        virtual ~Application();
        static Application& Get() { return *s_Instance; }

        void Run();
        virtual void OnEvent(Event& e);
        virtual void OnImGuiRender();;

        const Window& GetWindow() { return *m_Window; }
        const ImGuiViewportInfo& GetSceneViewportInfo() const { return m_SceneViewportInfo; }

    private:
        bool OnWindowClose(WindowCloseEvent& event);
        bool OnWindowResize(WindowResizeEvent& event);

    protected:
        scope<Window> m_Window;
        SceneManager m_SceneManager;

        bool m_Running = true;
        Timestep m_Delta = 0;

        ImGuiViewportInfo m_SceneViewportInfo;

    private:
        static inline Application* s_Instance = nullptr;
    };


    // implemented by the client
    Application* CreateApplication();

}

