#include "CameraController.h"
#include "Core/Input.h"
#include "Core/KeyCodes.h"
#include "Core/Core.h"
#include "Debug/Logger.h"
#include "Core/Application.h"
#include "Core/Timestep.h"
#include "Core/Window.h"
#include "Camera.h"
#include "Event/MouseEvent.h"
#include "Event/KeyEvent.h"
#include "Event/WindowEvent.h"
#include "Profiling/Instrumentor.h"

using namespace glm;

namespace ZPG {

    template<typename T> concept IsNumeric = std::is_integral_v<T> || std::is_floating_point_v<T>;

    template<IsNumeric T>
    static T wrap(T value, T low, T high)
    {
        if (value < low) return high;
        if (value > high) return low;
        return value;
    }

    CameraController::CameraController(Camera& camera)
        : m_Camera(camera)
    {
    }

    CameraController::~CameraController()
    {
    }

    void CameraController::OnUpdate(Timestep& ts)
    {
        ZPG_PROFILE_FUNCTION();
        // Update camera's position
        float translateSpeed = m_CameraTranslationSpeed * ts;

        vec3 cameraPosition = m_Camera.GetPosition();
        vec3 const& worldUp = m_Camera.GetWorldUp();
        vec3 const& right = m_Camera.GetRight();
        vec3 const& forward = m_Camera.GetForward();

        if (Input::IsKeyPressed(ZPG_KEY_LEFT_SHIFT)) {
            translateSpeed *= 10.0;
        }

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


        float mouseSensitivity = m_MouseSensitivity * ts * 100;

        if (Input::IsKeyPressed(ZPG_KEY_K))
            m_Pitch -= mouseSensitivity;
        if (Input::IsKeyPressed(ZPG_KEY_J))
            m_Pitch += mouseSensitivity;
        if (Input::IsKeyPressed(ZPG_KEY_H))
            m_Yaw -= mouseSensitivity;
        if (Input::IsKeyPressed(ZPG_KEY_L))
            m_Yaw += mouseSensitivity;

        m_Yaw = wrap(m_Yaw, -180.f, 180.f);
        m_Pitch = clamp(m_Pitch, -89.f, 89.f);

        // Update camera's orientation
        quat qYaw = angleAxis(radians(-m_Yaw), vec3(0, 1, 0));
        quat qPitch = angleAxis(radians(-m_Pitch), vec3(1, 0, 0));
        quat qRoll = angleAxis(radians(0.f), vec3(0, 0, -1));

        m_Camera.SetOrientation(normalize(qYaw * qPitch * qRoll));
    }

    void CameraController::OnEvent(Event& event)
    {
        ZPG_PROFILE_FUNCTION();

        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MouseScrolledEvent>(ZPG_FORWARD_EVENT_TO_MEMBER_FN(CameraController::OnMouseScrolled));
        dispatcher.Dispatch<MouseMovedEvent>(ZPG_FORWARD_EVENT_TO_MEMBER_FN(CameraController::OnMouseMoved));
    }

    void CameraController::OnResize(u32 width, u32 height)
    {
        ZPG_PROFILE_FUNCTION();

        if (width <= 0 || height <= 0)
            return;

        f32 aspectRatio = width / (float)height;
        m_Camera.SetPerspectiveProjection(m_Camera.GetFOV(), aspectRatio, m_Camera.GetZNear(), m_Camera.GetZFar());
    }

    bool CameraController::OnMouseMoved(MouseMovedEvent& e)
    {
        ZPG_PROFILE_FUNCTION();

        if (Input::IsMouseButtonPressed(ZPG_MOUSE_BUTTON_MIDDLE))
        {
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

            m_Camera.SetOrientation(normalize(qYaw * qPitch * qRoll));

            m_LastCursorPos = currentPos;
        }
        else
        {
            m_LastCursorPos = vec2(e.GetXPos(), e.GetYPos());
        }

        return false;
    }

    bool CameraController::OnMouseScrolled(MouseScrolledEvent& e)
    {
        ZPG_PROFILE_FUNCTION();

        constexpr f32 sensitivity = 5.0f;

        f32 fov = m_Camera.GetFOV() + sensitivity * e.GetYOffset();

        m_Camera.SetFOV(fov);
        m_Camera.CalcPerspectiveProjection();

        return false;
    }

    bool CameraController::OnWindowResized(WindowResizeEvent& e)
    {
        ZPG_PROFILE_FUNCTION();
        Input::ShowCursor();
        f32 aspectRatio = e.GetWidth() / (float)e.GetHeight();
        m_Camera.SetPerspectiveProjection(m_Camera.GetFOV(), aspectRatio, m_Camera.GetZNear(), m_Camera.GetZFar());
        return false;
    }

}
