#include "Camera.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

namespace ZPG {

Camera::Camera(const glm::vec3& position, const glm::quat& orientation) : m_Position(position) 
, m_Orientation(orientation)
, m_ProjMatrix(mat4(1.f))
, m_ViewMatrix(glm::mat4(1.f))
, m_ViewProjMatrix(glm::mat4(1.f)) {
    Recalculate();
    SetPerspectiveProjection(m_FOV, m_AspectRatio, m_zNear, m_zFar);
    m_ViewProjMatrix = m_ProjMatrix * m_ViewMatrix;
}
Camera::~Camera() {

}
void Camera::SetOrthoProjection(f32 left, f32 right, f32 bottom, f32 top) {
    m_OrthoLeft = left;
    m_OrthoRight = right;
    m_OrthoBottom = bottom;
    m_OrthoTop = top;
    m_ProjMatrix = glm::ortho(left, right, bottom, top);
    m_ViewProjMatrix = m_ProjMatrix * m_ViewProjMatrix;
}
void Camera::SetPerspectiveProjection(f32 fovAngle, f32 aspectRatio, f32 zNear, f32 zFar) {
    m_FOV = fovAngle;
    m_zNear = zNear;
    m_zFar = zFar;
    m_AspectRatio = aspectRatio;
    m_ProjMatrix = glm::perspective(glm::radians(fovAngle), aspectRatio, zNear, zFar);
    m_ViewProjMatrix = m_ProjMatrix * m_ViewProjMatrix;
}
void Camera::CalcSetOrthoProjection() {
    SetOrthoProjection(m_OrthoLeft, m_OrthoRight, m_OrthoBottom, m_OrthoTop);
}
void Camera::CalcPerspectiveProjection() {
    SetPerspectiveProjection(m_FOV, m_AspectRatio, m_zNear, m_zFar);
}
const glm::vec3& Camera::GetPosition() const {
    return m_Position;
}
void Camera::SetPosition(const glm::vec3& position) {
    m_Position = position;
    Recalculate();
}
const glm::quat& Camera::GetOrientation() const {
    return m_Orientation;
}
void Camera::SetOrientation(const glm::quat& orientation) {
    m_Orientation = orientation;
    Recalculate();
}
const glm::mat4& Camera::GetViewMatrix() const {
    return m_ViewMatrix;
}
const glm::mat4& Camera::GetProjMatrix() const {
    return m_ProjMatrix;
}
const glm::mat4& Camera::GetViewProjMatrix() const {
    return m_ViewProjMatrix;
}
void Camera::Recalculate() {
    using namespace glm;

    m_Front = rotate(m_Orientation, vec3(0.0, 0.0, -1.0));
    m_Right = rotate(m_Orientation, vec3(1.0, 0.0, 0.0));
    m_Forward = cross(s_WorldUp, m_Right);

    m_ViewMatrix = lookAt(m_Position, m_Position + m_Front, s_WorldUp);
    m_ViewProjMatrix = m_ProjMatrix * m_ViewMatrix;
}

}
