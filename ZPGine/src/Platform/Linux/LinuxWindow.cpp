#include "LinuxWindow.h"
#include "Core/Window.h"
#include "Debug/Asserter.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Event/WindowEvent.h"
#include "Event/MouseEvent.h"
#include "Event/KeyEvent.h"
#include "Profiling/Instrumentor.h"

namespace ZPG {

static bool s_GLFWInitialised = false;
static void CustomGLFWErrorCallback(int error_code, const char* description) {
    ZPG_PROFILE_FUNCTION();
    ZPG_CORE_ERROR("GLFW Error {}: {}", error_code, description);
}

LinuxWindow::LinuxWindow(const WindowProps& props) 
    : m_Data({.Props = props, .EventCallback = nullptr}) 
{
    ZPG_PROFILE_FUNCTION();
    Init();
}

void LinuxWindow::Init() {
    ZPG_PROFILE_FUNCTION();
    ZPG_CORE_INFO("Creating window: {} size = ({}, {})", m_Data.Props.Title, m_Data.Props.Width, m_Data.Props.Height);

    // GLFW can be init only once
    if (!s_GLFWInitialised) {
        bool success = glfwInit();
        ZPG_CORE_ASSERT(success, "Couldn't initialised GLFW.");
        glfwSetErrorCallback(CustomGLFWErrorCallback);
        s_GLFWInitialised = true;
    }

    // i3 will make windows with this hint "floating", but not resizable
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    // make sure it has a depth buffer
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    // Create GLFW window    
    m_Window = glfwCreateWindow(m_Data.Props.Width, m_Data.Props.Height, m_Data.Props.Title.c_str(), NULL, NULL);
    // bring back resize-ability
    glfwSetWindowAttrib(m_Window, GLFW_RESIZABLE, GLFW_TRUE);

    // Create OpenGL context
    m_Context = new OpenGLContext(m_Window);
    m_Context->Init();

    // custom pointer data passed to GLFW, can be retrieved in callback functions
    glfwSetWindowUserPointer(m_Window, &m_Data);

    SetVSync(m_Data.Props.VSync);

    SetupCallbacks();
}
void LinuxWindow::SetupCallbacks() {
    ZPG_PROFILE_FUNCTION();
    // set window callbacks
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) -> void {
        auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        data->Props.Width = width;
        data->Props.Height = height;
        WindowResizeEvent event(width, height);
        data->EventCallback(event);
    });

    glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int xpos, int ypos) -> void {
        auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        WindowMovedEvent event(xpos, ypos);
        data->EventCallback(event);
    });

    glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused){
        auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        switch (focused) {
        case GLFW_TRUE: {
            WindowFocusEvent event;
            data->EventCallback(event);
        } break;
        case GLFW_FALSE: {
            WindowLostFocusEvent event;
            data->EventCallback(event);
        } break;
        default: break;
        }
    });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) -> void {
        auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        WindowCloseEvent event{};
        data->EventCallback(event);
    });

    // set keyboard callbacks
    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) -> void {
        auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        switch (action) {
        case GLFW_PRESS: {
            KeyPressedEvent event(key, 0);
            data->EventCallback(event);
        } break;
        case GLFW_REPEAT: {
            KeyPressedEvent event(key, 1);
            data->EventCallback(event);
        } break;
        case GLFW_RELEASE: {
            KeyReleasedEvent event(key);
            data->EventCallback(event);
        } break;
        default: break;
        }
    });

    glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int charCode) {
        auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        KeyTypedEvent event(charCode);
        data->EventCallback(event);
    });

    // set mouse callbacks
    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) -> void {
        auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        switch (action) {
        case GLFW_PRESS: {
            MouseButtonPressedEvent event(button);
            data->EventCallback(event);
        } break;
        case GLFW_RELEASE: {
            MouseButtonReleasedEvent event(button);
            data->EventCallback(event);
        } break;
        default: break;
        }
    });

    glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) -> void {
        auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        MouseScrolledEvent event((float)xOffset, (float)yOffset);
        data->EventCallback(event);
    });

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y) -> void {
        auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        MouseMovedEvent event((float)x, (float)y);
        data->EventCallback(event);
    });
}

void LinuxWindow::Shutdown() {
    ZPG_PROFILE_FUNCTION();
    glfwDestroyWindow(m_Window);
}

LinuxWindow::~LinuxWindow() {
    ZPG_PROFILE_FUNCTION();
    delete m_Context;
    Shutdown();
}

void LinuxWindow::OnUpdate() {
    ZPG_PROFILE_FUNCTION();
    glfwPollEvents();
    m_Context->SwapBuffers();
}

void LinuxWindow::SetEventCallback(EventCallbackFn&& fn) {
    ZPG_PROFILE_FUNCTION();
    m_Data.EventCallback = fn;
}

u32 LinuxWindow::GetWidth() const {
    ZPG_PROFILE_FUNCTION();
    return m_Data.Props.Width;
}

u32 LinuxWindow::GetHeight() const {
    ZPG_PROFILE_FUNCTION();
    return m_Data.Props.Height;
}

void LinuxWindow::SetVSync(bool enabled) {
    ZPG_PROFILE_FUNCTION();
    if (enabled) glfwSwapInterval(1);
    else glfwSwapInterval(0);
    m_Data.Props.VSync = enabled;
}

bool LinuxWindow::IsVSync() const {
    ZPG_PROFILE_FUNCTION();
    return m_Data.Props.VSync;
}

void* LinuxWindow::GetNativeWindow() const {
    ZPG_PROFILE_FUNCTION();
    return m_Window;
}

}
