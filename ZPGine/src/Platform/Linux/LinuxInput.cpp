#include "LinuxInput.h"
#include "Core/Application.h"
#include "Core/Window.h"
#include "Profiling/Instrumentor.h"

namespace ZPG 
{

    LinuxInput::LinuxInput() 
    {
    }

    LinuxInput::~LinuxInput() 
    {
    }

    bool LinuxInput::IsKeyPressedImpl(int keyCode) 
    {
        ZPG_PROFILE_FUNCTION();
        GLFWwindow* w = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
        i32 state = glfwGetKey(w, keyCode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool LinuxInput::IsMouseButtonPressedImpl(int buttonCode)   
    {
        ZPG_PROFILE_FUNCTION();
        GLFWwindow* w = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
        i32 state = glfwGetMouseButton(w, buttonCode);
        return state == GLFW_PRESS;
    }

    v2 LinuxInput::GetMousePositionImpl()  
    {
        ZPG_PROFILE_FUNCTION();
        GLFWwindow* w = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
        f64 x, y;
        glfwGetCursorPos(w, &x, &y);
        return v2{x,y};
    }

    void LinuxInput::ShowCursorImpl() 
    {
        ZPG_PROFILE_FUNCTION();
        GLFWwindow* w = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
        glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void LinuxInput::HideCursorImpl() 
    {
        ZPG_PROFILE_FUNCTION();
        GLFWwindow* w = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
        glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }

    void LinuxInput::GrabCursorImpl() 
    {
        ZPG_PROFILE_FUNCTION();
        GLFWwindow* w = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
        glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void LinuxInput::ConfineCursorImpl() 
    {
        ZPG_PROFILE_FUNCTION();
        GLFWwindow* w = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
        glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
    }

    bool LinuxInput::IsCursorShownImpl()
    {
        ZPG_PROFILE_FUNCTION();
        GLFWwindow* w = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
        return glfwGetInputMode(w, GLFW_CURSOR) == GLFW_CURSOR_NORMAL;
    }

    bool LinuxInput::IsCursorHiddenImpl() 
    {
        ZPG_PROFILE_FUNCTION();
        GLFWwindow* w = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
        return glfwGetInputMode(w, GLFW_CURSOR) == GLFW_CURSOR_HIDDEN;
    }

    bool LinuxInput::IsCursorGrabbedImpl() 
    {
        ZPG_PROFILE_FUNCTION();
        GLFWwindow* w = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
        return glfwGetInputMode(w, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
    }

    bool LinuxInput::IsCursorConfinedImpl() 
    {
        ZPG_PROFILE_FUNCTION();
        GLFWwindow* w = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
        return glfwGetInputMode(w, GLFW_CURSOR) == GLFW_CURSOR_CAPTURED;
    }
}
