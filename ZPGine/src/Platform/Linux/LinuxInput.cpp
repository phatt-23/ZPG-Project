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

}
