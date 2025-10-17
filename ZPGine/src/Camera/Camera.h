//
// Created by phatt on 9/22/25.
//

#ifndef CAMERA_H
#define CAMERA_H

namespace ZPG {

class Camera {
public:
    Camera(const v3& position = v3(0.0, 0.0, 1.0), 
        const qtr& orientation = glm::quatLookAt(v3(0.f, 0.f, -1.f), s_WorldUp));

    ~Camera();

    void SetOrthoProjection(f32 left, f32 right, f32 bottom, f32 top);
    void SetPerspectiveProjection(f32 fov, f32 aspectRatio, f32 zNear, f32 zFar);

    const v3& GetPosition() const;
    void SetPosition(const glm::vec3& position);
    const qtr& GetOrientation() const;
    void SetOrientation(const glm::quat& orientation);

    const m4& GetViewMatrix() const;
    const m4& GetProjMatrix() const;
    const m4& GetViewProjMatrix() const;

    f32 GetFOV() const { return m_FOV; }
    f32 GetZNear() const { return m_zNear; }
    f32 GetZFar() const { return m_zFar; }

    const v3& GetFront() const { return m_Front; }
    const v3& GetForward() const { return m_Forward; }
    const v3& GetRight() const { return m_Right; }
    const v3& GetWorldUp() const { return s_WorldUp; }
private:
    void Recalculate();

    m4 m_ViewMatrix;
    m4 m_ProjMatrix;
    m4 m_ViewProjMatrix;

    v3 m_Position;
    qtr m_Orientation;

    // lens specs
    // perspective
    f32 m_FOV = 45.0f; // expressed in angles
    f32 m_AspectRatio = 0.5f;
    f32 m_zNear = 0.1f;  // if 0 then behaves weird, clipping
    f32 m_zFar = 100.0f;
    // ortho
    f32 m_OrthoLeft = -1.f;
    f32 m_OrthoRight = 1.0f;
    f32 m_OrthoBottom = -1.0f;
    f32 m_OrthoTop = 1.0f;

    // cache
    static constexpr v3 s_WorldUp = v3(0.0, 1.0, 0.0);
    v3 m_Front;  // direction camera is pointing at
    v3 m_Right;  // right hand of camera
    v3 m_Forward;  // forward in xy-plane

    f32 m_Yaw = 0.f;
    f32 m_Pitch = 0.f;
};

}

#endif //CAMERA_H
