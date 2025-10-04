//
// Created by phatt on 9/22/25.
//

#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"
#include "Event/KeyEvent.h"
#include "Core/Timestep.h"

#include "Camera.h"

namespace ZPG {
class CameraController {
public:
    CameraController(Camera& camera);
    virtual ~CameraController();
    
    virtual void OnUpdate(Timestep ts);
    virtual void OnEvent(Event& e);

    Camera& GetCamera() { return m_Camera; }
    const Camera& GetCamera() const { return m_Camera; }
private:
    bool OnWindowFocus(WindowFocusEvent& e);
    bool OnWindowLostFocus(WindowLostFocusEvent& e);
    bool OnWindowResized(WindowResizeEvent& e);
    bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
    bool OnMouseMoved(MouseMovedEvent& e);
    bool OnMouseScrolled(MouseScrolledEvent& e);
    bool OnKeyPressed(KeyPressedEvent& e);
private:
    Camera& m_Camera;

    f32 m_CameraTranslationSpeed = 2.0f;
    f32 m_MouseSensitivity = 1.f;
    
    glm::vec2 m_LastCursorPos = {0.0, 0.0};
    f32 m_Yaw = 0.f;
    f32 m_Pitch = 0.f;
};
}

#endif //CAMERACONTROLLER_H
