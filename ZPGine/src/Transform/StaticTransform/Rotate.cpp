#include "Rotate.h"
#include "Transform/StaticTransform.h"
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Translate.h"

using namespace glm;

namespace ZPG {

static constexpr glm::vec3 c_Origin = glm::vec3(0.0);

Rotate::Rotate(const glm::vec3& direction, const glm::vec3& up, const glm::vec3& origin)
: StaticTransform(false)
, m_RotationQuaternion(quatLookAt(normalize(direction), normalize(up)))
, m_Origin(origin)
{
    ComputeMatrix();
}

Rotate::Rotate(const glm::quat& rotation, const glm::vec3& origin)
: StaticTransform(false)
, m_RotationQuaternion(rotation)
, m_Origin(origin)
{
    ComputeMatrix();
}

Rotate::Rotate(f32 rotationAngleDeg, const glm::vec3& rotationAxis, const glm::vec3& origin)
: StaticTransform(false)
, m_RotationQuaternion(
    glm::angleAxis(
        glm::radians(rotationAngleDeg),
        glm::normalize(rotationAxis)
    ))
, m_Origin(origin)
{
    ComputeMatrix();
}
void Rotate::ComputeMatrix() {
    glm::mat4 R = glm::mat4_cast(m_RotationQuaternion);

    if (m_Origin != c_Origin) {
        // go out from origin
        glm::mat4 T = glm::translate(glm::mat4(1.0f), m_Origin);
        // inverse, get back to origin
        glm::mat4 Ti = glm::translate(glm::mat4(1.0f), -m_Origin);
        R =  T * R * Ti;
    }

    m_Matrix = R;
    m_Cached = true;
}


}
