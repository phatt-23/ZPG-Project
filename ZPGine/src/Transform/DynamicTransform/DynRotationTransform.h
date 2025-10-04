#pragma once 
#include "Transform/DynTransform.h"
#include <glm/gtc/quaternion.hpp>
#include "Core/Core.h"

namespace ZPG {

class DynRotationTransform : public DynamicTransform {
public:
    DynRotationTransform(
        f32 startingRotationDeg, 
        f32 rotationSpeedDeg, 
        const glm::vec3& rotationAxis);

    void Update(Timestep ts) override;
private:
    void ComputeMatrix() override;
private:
    f32 m_CurrentRotationDeg;
    f32 m_RotationSpeedDeg;  // angle per second
    glm::vec3 m_RotationAxis;
};

}
