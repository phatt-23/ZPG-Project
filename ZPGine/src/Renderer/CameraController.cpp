#include "CameraController.h"
#include "Core/Input.h"
#include "Core/KeyCodes.h"
#include <glm/ext/quaternion_common.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Core/Core.h"
#include "Debug/Logger.h"
#include <GLFW/glfw3.h>
#include "Core/Application.h"
#include "imgui.h"

using namespace glm;

namespace ZPG {

template<typename T> concept IsNumeric = std::is_integral_v<T> || std::is_floating_point_v<T>;

template<IsNumeric T>
static T wrap(T value, T low, T high) {
    if (value < low) return high;
    if (value > high) return low;
    return value;
}

CameraController::CameraController() 
: m_Camera() {

}
CameraController::~CameraController() {

}
void CameraController::OnUpdate(Timestep ts) {
    // Update camera's position
    float translateSpeed = m_CameraTranslationSpeed * ts;
    vec3 cameraPosition = m_Camera.GetPosition();
    quat orientation = m_Camera.GetOrientation();
    vec3 front, right, worldUp, forward;
    
    front = rotate(orientation, vec3(0.0, 0.0, -1.0));
    right = rotate(orientation, vec3(1.0, 0.0, 0.0));
    worldUp = vec3(0.0, 1.0, 0.0);

    forward = cross(worldUp, right);

    if (Input::IsKeyPressed(ZPG_KEY_W))
        cameraPosition += forward * translateSpeed;
    if (Input::IsKeyPressed(ZPG_KEY_A))
        cameraPosition -= right * translateSpeed;
    if (Input::IsKeyPressed(ZPG_KEY_S))
        cameraPosition -= forward * translateSpeed;
    if (Input::IsKeyPressed(ZPG_KEY_D))
        cameraPosition += right * translateSpeed;
    if (Input::IsKeyPressed(ZPG_KEY_SPACE))
        cameraPosition += worldUp * translateSpeed;
    if (Input::IsKeyPressed(ZPG_KEY_LEFT_CONTROL))
        cameraPosition -= worldUp * translateSpeed;
    
    m_Camera.SetPosition(cameraPosition);
}
void CameraController::OnEvent(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<MouseScrolledEvent>(ZPG_FORWARD_EVENT_TO_MEMBER_FN(CameraController::OnMouseScrolled));
    dispatcher.Dispatch<WindowResizeEvent>(ZPG_FORWARD_EVENT_TO_MEMBER_FN(CameraController::OnWindowResized));
    dispatcher.Dispatch<MouseMovedEvent>(ZPG_FORWARD_EVENT_TO_MEMBER_FN(CameraController::OnMouseMoved));
    dispatcher.Dispatch<MouseButtonPressedEvent>(ZPG_FORWARD_EVENT_TO_MEMBER_FN(CameraController::OnMouseButtonPressed));
    dispatcher.Dispatch<KeyPressedEvent>(ZPG_FORWARD_EVENT_TO_MEMBER_FN(CameraController::OnKeyPressed));
    dispatcher.Dispatch<WindowLostFocusEvent>(ZPG_FORWARD_EVENT_TO_MEMBER_FN(CameraController::OnWindowLostFocus));
    dispatcher.Dispatch<WindowFocusEvent>(ZPG_FORWARD_EVENT_TO_MEMBER_FN(CameraController::OnWindowFocus));
    dispatcher.Dispatch<WindowResizeEvent>(ZPG_FORWARD_EVENT_TO_MEMBER_FN(CameraController::OnWindowResize));
}
bool CameraController::OnWindowFocus(WindowFocusEvent& e) {
    return false;
}
bool CameraController::OnWindowResize(WindowResizeEvent& e) {
    Input::ShowCursor();
    return false;
}
bool CameraController::OnWindowLostFocus(WindowLostFocusEvent& e) {
    Input::ShowCursor();
    return false;
}
bool CameraController::OnKeyPressed(KeyPressedEvent& e) {
    return false;
}
bool CameraController::OnMouseMoved(MouseMovedEvent& e) {
    if (Input::IsCursorGrabbed()) {

        vec2 currentPos = vec2(e.GetXPos(), e.GetYPos());
        vec2 offset = currentPos - m_LastCursorPos;
    
        m_Yaw += offset.x * m_MouseSensitivity;
        m_Yaw = wrap(m_Yaw, -180.f, 180.f);
        m_Pitch += offset.y * m_MouseSensitivity;
        m_Pitch = clamp(m_Pitch, -89.f, 89.f);
        
        // Update camera's orientation
        quat qYaw = angleAxis(radians(-m_Yaw), vec3(0.0, 1.0, 0.0));
        quat qPitch = angleAxis(radians(-m_Pitch), vec3(1.0, 0.0, 0.0));
        quat qRoll = angleAxis(radians(0.f), vec3(0.0, 0.0, -1.0));
        
        // ZPG_CORE_DEBUG("YAW: {}, PITCH: {}", m_Yaw, m_Pitch);
        m_Camera.SetOrientation(normalize(qYaw * qPitch * qRoll));
        
        m_LastCursorPos = currentPos;
    }
    return false;
}
bool CameraController::OnMouseButtonPressed(MouseButtonPressedEvent& e) {
    GLFWwindow* w = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
    if (e.GetButtonCode() == ZPG_MOUSE_BUTTON_LEFT) {
        Input::GrabCursor();
        m_LastCursorPos = vec2(Input::GetMouseX(), Input::GetMouseY());
    }
    if (e.GetButtonCode() == ZPG_MOUSE_BUTTON_RIGHT) {
        Input::ShowCursor();
    }

    return false;
}
bool CameraController::OnMouseScrolled(MouseScrolledEvent& e) {
    ZPG_CORE_DEBUG("{}: On mouse scroll", __PRETTY_FUNCTION__);
    return false;
}
bool CameraController::OnWindowResized(WindowResizeEvent& e) {
    f32 aspectRatio = e.GetWidth() / (float)e.GetHeight();
    m_Camera.SetPerspectiveProjection(m_Camera.GetFOV(), aspectRatio, m_Camera.GetZNear(), m_Camera.GetZFar());
    return false;
}

}
