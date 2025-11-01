#include "Rotate.h"
#include "Transform/StaticTransform.h"
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Translate.h"

namespace ZPG {

Rotate::Rotate(const v3& direction, const v3& up)
: StaticTransform(false)
, m_RotationQuaternion(quatLookAt(normalize(direction), normalize(up)))
{
    ComputeMatrix();
}

Rotate::Rotate(const qtr& rotation)
: StaticTransform(false)
, m_RotationQuaternion(rotation)
{
    ComputeMatrix();
}

Rotate::Rotate(f32 rotationAngleDeg, const v3& rotationAxis)
: StaticTransform(false)
, m_RotationQuaternion( glm::angleAxis( glm::radians(rotationAngleDeg), glm::normalize(rotationAxis) ) )
{
    ComputeMatrix();
}
void Rotate::ComputeMatrix() {
    m_Matrix = glm::mat4_cast(m_RotationQuaternion);
    m_Cached = true;
}

const qtr& Rotate::GetRotation() const {
    return m_RotationQuaternion;
}

void Rotate::SetRotation(const qtr& rotation) {
    m_RotationQuaternion = rotation;
    ComputeMatrix();
}

}
