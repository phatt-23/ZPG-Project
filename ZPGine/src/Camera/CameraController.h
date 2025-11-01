//
// Created by phatt on 9/22/25.
//

#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H


namespace ZPG {

class WindowFocusEvent;
class WindowResizeEvent;
class WindowLostFocusEvent;
class MouseButtonPressedEvent;
class MouseMovedEvent;
class MouseScrolledEvent;
class KeyPressedEvent;
class Event;
class Timestep;
class Camera;

class CameraController {
public:
    CameraController(Camera& camera);
    virtual ~CameraController();
    
    virtual void OnUpdate(Timestep& ts);
    virtual void OnEvent(Event& e);

    Camera& GetCamera() { return m_Camera; }
    const Camera& GetCamera() const { return m_Camera; }

    void OnResize(u32 width, u32 height);

private:
    bool OnWindowFocus(WindowFocusEvent& e);
    bool OnWindowLostFocus(WindowLostFocusEvent& e);
    bool OnWindowResized(WindowResizeEvent& e);
    bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
    bool OnMouseMoved(MouseMovedEvent& e);
    bool OnMouseScrolled(MouseScrolledEvent& e);
    bool OnKeyPressed(KeyPressedEvent& e);
protected:
    Camera& m_Camera;

    f32 m_CameraTranslationSpeed = 2.0f;
    f32 m_MouseSensitivity = 1.f;
    
    glm::vec2 m_LastCursorPos = {0.0, 0.0};
    f32 m_Yaw = 0.f;
    f32 m_Pitch = 0.f;
};
}

#endif //CAMERA_CONTROLLER_H
