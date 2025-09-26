#include "Camera.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ZPG {

Camera::Camera(const glm::vec3& position, const glm::quat& orientation) 
: m_ProjMatrix(glm::identity<glm::mat4>())
, m_ViewMatrix(glm::identity<glm::mat4>())
, m_ViewProjMatrix(glm::identity<glm::mat4>())
, m_Orientation(orientation)
, m_Position(position) {
    RecalculateViewMatrix();
    m_ViewProjMatrix = m_ProjMatrix * m_ViewMatrix;
}
Camera::~Camera() {

}
void Camera::SetOrthoProjection(f32 left, f32 right, f32 bottom, f32 top) {
    m_ProjMatrix = glm::ortho(left, right, bottom, top);
    m_ViewProjMatrix = m_ProjMatrix * m_ViewProjMatrix;
}
void Camera::SetPerspectiveProjection(f32 fov, f32 aspectRatio, f32 zNear, f32 zFar) {
    m_FOV = fov;
    m_zNear = zNear;
    m_zFar = zFar;
    m_AspectRatio = aspectRatio;
    m_ProjMatrix = glm::perspective(fov, aspectRatio, zNear, zFar);
    m_ViewProjMatrix = m_ProjMatrix * m_ViewProjMatrix;
}
const glm::vec3& Camera::GetPosition() const {
    return m_Position;
}
void Camera::SetPosition(const glm::vec3& position) {
    m_Position = position;
    RecalculateViewMatrix();
}
const glm::quat& Camera::GetOrientation() const {
    return m_Orientation;
}
const void Camera::SetOrientation(const glm::quat& orientation) {
    m_Orientation = orientation;
    RecalculateViewMatrix();
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
void Camera::RecalculateViewMatrix() {
    using namespace glm;

    // m_ViewMatrix = inverse(translate(mat4(1), m_Position) * toMat4(m_Orientation));
    vec3 front, right, up;
    
    front = rotate(m_Orientation, vec3(0.0, 0.0, -1.0));
    right = rotate(m_Orientation, vec3(1.0, 0.0, 0.0));
    up = vec3(0.0, 1.0, 0.0);
    m_ViewMatrix = lookAt(m_Position, m_Position + front, up);

    m_ViewProjMatrix = m_ProjMatrix * m_ViewMatrix;
}

}
