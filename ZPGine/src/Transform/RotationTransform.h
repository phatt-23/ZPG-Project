#pragma once 
#include "Transformation.h"
#include <glm/gtc/quaternion.hpp>
#include "Core/Core.h"

namespace ZPG {

class RotationTransform : public Transform {
public:
    RotationTransform(const glm::vec3& direction, const glm::vec3& up = glm::vec3(0.f, 1.f, 0.f));
    RotationTransform(const glm::quat& rotation);  /// glm::quatLookAt(glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f)))
    RotationTransform(f32 rotationAngleDeg, const glm::vec3& rotationAxis);

    glm::mat4 GetMatrix() override;
private:
    glm::quat m_RotationQuaternion;
};

}
