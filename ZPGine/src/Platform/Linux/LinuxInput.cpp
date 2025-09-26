#include "LinuxInput.h"
#include "Core/Application.h"
#include <GLFW/glfw3.h>

namespace ZPG {

LinuxInput::LinuxInput() {
}
LinuxInput::~LinuxInput() {
}

bool LinuxInput::IsKeyPressedImpl(int keyCode) {
    GLFWwindow* w = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
    i32 state = glfwGetKey(w, keyCode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}
bool LinuxInput::IsMouseButtonPressedImpl(int buttonCode) {
    GLFWwindow* w = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
    i32 state = glfwGetMouseButton(w, buttonCode);
    return state == GLFW_PRESS;
}
std::pair<float, float> LinuxInput::GetMousePositionImpl()  {
    GLFWwindow* w = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
    f64 x, y;
    glfwGetCursorPos(w, &x, &y);
    return {x,y};
}
void LinuxInput::ShowCursorImpl() {
    GLFWwindow* w = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
    glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
void LinuxInput::HideCursorImpl() {
    GLFWwindow* w = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
    glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}
void LinuxInput::GrabCursorImpl() {
    GLFWwindow* w = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
    glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
void LinuxInput::ConfineCursorImpl() {
    GLFWwindow* w = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
    glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
}
bool LinuxInput::IsCursorShownImpl()
{
    GLFWwindow* w = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
    return glfwGetInputMode(w, GLFW_CURSOR) == GLFW_CURSOR_NORMAL;
}
bool LinuxInput::IsCursorHiddenImpl() {
    GLFWwindow* w = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
    return glfwGetInputMode(w, GLFW_CURSOR) == GLFW_CURSOR_HIDDEN;
}
bool LinuxInput::IsCursorGrabbedImpl() {
    GLFWwindow* w = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
    return glfwGetInputMode(w, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
}
bool LinuxInput::IsCursorConfinedImpl() {
    GLFWwindow* w = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
    return glfwGetInputMode(w, GLFW_CURSOR) == GLFW_CURSOR_CAPTURED;
}
}
