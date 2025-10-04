#include "RotationTransform.h"
#include "Transform/StaticTransform.h"
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;

namespace ZPG {

RotationTransform::RotationTransform(const glm::vec3& direction, const glm::vec3& up) 
: StaticTransform(false) {
    m_RotationQuaternion = quatLookAt(normalize(direction), normalize(up));
    ComputeMatrix();
}

RotationTransform::RotationTransform(const glm::quat& rotation) 
: StaticTransform(false) {
    m_RotationQuaternion = rotation;
    ComputeMatrix();
}

RotationTransform::RotationTransform(f32 rotationAngleDeg, const glm::vec3& rotationAxis) 
: StaticTransform(false) {
    m_RotationQuaternion = glm::angleAxis(
        glm::radians(rotationAngleDeg),
        glm::normalize(rotationAxis)
    );
    ComputeMatrix();
}
void RotationTransform::ComputeMatrix(){
    m_Matrix = glm::mat4_cast(m_RotationQuaternion);
    m_Cached = true;
}


}
