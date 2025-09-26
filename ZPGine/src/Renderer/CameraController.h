//
// Created by phatt on 9/22/25.
//

#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"
#include "Core/Timestep.h"

#include "Camera.h"

namespace ZPG {
class CameraController {
public:
    CameraController();
    ~CameraController();

    Camera& GetCamera() { return m_Camera; }
    const Camera& GetCamera() const { return m_Camera; }

    void OnUpdate(Timestep ts);
    void OnEvent(Event& e);

private:
    bool OnMouseScrolled(MouseScrolledEvent& e);
    bool OnWindowResized(WindowResizeEvent& e);
    bool OnMouseMoved(MouseMovedEvent& e);

private:
    Camera m_Camera;

    f32 m_AspectRatio = 0.0f;

    f32 m_CameraTranslationSpeed = 2.0f;

    glm::vec2 m_LastCursorPos = {0.0, 0.0};
    f32 m_Yaw = 0.f;
    f32 m_Pitch = 0.f;
    f32 m_MouseSensitivity = 1.f;

};
}

#endif //CAMERACONTROLLER_H
