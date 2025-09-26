//
// Created by phatt on 9/22/25.
//

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Core/Core.h"

namespace ZPG {

class Camera {
public:
    Camera(const glm::vec3& position = glm::vec3(0.f), 
        const glm::quat& orientation = glm::quatLookAt(glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f)));
    ~Camera();

    void SetOrthoProjection(f32 left, f32 right, f32 bottom, f32 top);
    void SetPerspectiveProjection(f32 fov, f32 aspectRatio, f32 zNear, f32 zFar);

    const glm::vec3& GetPosition() const;
    void SetPosition(const glm::vec3& position);
    const glm::quat& GetOrientation() const;
    void SetOrientation(const glm::quat& orientation);

    const glm::mat4& GetViewMatrix() const;
    const glm::mat4& GetProjMatrix() const;
    const glm::mat4& GetViewProjMatrix() const;

    f32 GetFOV() const { return m_FOV; }
    f32 GetZNear() const { return m_zNear; }
    f32 GetZFar() const { return m_zFar; }
private:
    void RecalculateViewMatrix();

    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ProjMatrix;
    glm::mat4 m_ViewProjMatrix;

    glm::vec3 m_Position;
    glm::quat m_Orientation;

    // lens specs
    // perspective
    f32 m_FOV = 45.0f;
    f32 m_AspectRatio = 0.5f;
    f32 m_zNear = 0.1f;  // if 0 then behaves weird, clipping
    f32 m_zFar = 100.0f;
    // ortho
    f32 m_Left = -1.f;
    f32 m_Right = 1.0f;
    f32 m_Bottom = -1.0f;
    f32 m_Top = 1.0f;
};

}

#endif //CAMERA_H
