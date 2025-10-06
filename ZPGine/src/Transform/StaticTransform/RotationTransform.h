#pragma once 
#include "Transform/StaticTransform.h"
#include <glm/gtc/quaternion.hpp>
#include "Core/Core.h"

namespace ZPG {

class RotationTransform : public StaticTransform {
public:
    RotationTransform(const glm::vec3& direction, const glm::vec3& up = glm::vec3(0.f, 1.f, 0.f));
    RotationTransform(const glm::quat& rotation);  
    RotationTransform(f32 rotationAngleDeg, const glm::vec3& rotationAxis);
    void ComputeMatrix() override;
private:
    glm::quat m_RotationQuaternion;
};

}
