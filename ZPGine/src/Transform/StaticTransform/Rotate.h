#pragma once 
#include "Transform/StaticTransform.h"
#include <glm/gtc/quaternion.hpp>
#include "Core/Core.h"

namespace ZPG {

class Rotate : public StaticTransform {
public:
    Rotate(const glm::vec3& direction, const glm::vec3& up = glm::vec3(0.f, 1.f, 0.f), const glm::vec3& origin = glm::vec3(0.0));
    Rotate(const glm::quat& rotation, const glm::vec3& origin = glm::vec3(0.0));
    Rotate(f32 rotationAngleDeg, const glm::vec3& rotationAxis, const glm::vec3& origin = glm::vec3(0.0));
    void ComputeMatrix() override;
private:
    glm::quat m_RotationQuaternion;
    glm::vec3 m_Origin;
};

}
