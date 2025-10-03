#include "RotationTransform.h"
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;

namespace ZPG {

RotationTransform::RotationTransform(const glm::vec3& direction, const glm::vec3& up) {
    m_RotationQuaternion = quatLookAt(normalize(direction), normalize(up));
}

RotationTransform::RotationTransform(const glm::quat& rotation) {
    m_RotationQuaternion = rotation;
}

RotationTransform::RotationTransform(f32 rotationAngleDeg, const glm::vec3& rotationAxis) {
    m_RotationQuaternion = glm::angleAxis(
        glm::radians(rotationAngleDeg),
        glm::normalize(rotationAxis)
    );
}

glm::mat4 RotationTransform::GetMatrix() {
    return glm::mat4_cast(m_RotationQuaternion);
}

}
