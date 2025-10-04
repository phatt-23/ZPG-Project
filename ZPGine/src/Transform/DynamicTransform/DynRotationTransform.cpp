#include "DynRotationTransform.h"
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

namespace ZPG {

DynRotationTransform::DynRotationTransform(f32 startingRotationDeg, f32 rotationSpeedDeg, const glm::vec3& rotationAxis)
: m_CurrentRotationDeg(startingRotationDeg)
, m_RotationSpeedDeg(rotationSpeedDeg)
, m_RotationAxis(rotationAxis) {
    ComputeMatrix();
}

void DynRotationTransform::Update(Timestep ts) {
    m_CurrentRotationDeg += m_RotationSpeedDeg * ts.AsSeconds();
    ComputeMatrix();
}

void DynRotationTransform::ComputeMatrix() {
    m_Matrix = glm::rotate(glm::mat4(1.0f), glm::radians(m_CurrentRotationDeg), m_RotationAxis);
}

}
