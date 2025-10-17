#include "DynRotate.h"
#include "Core/Timestep.h"

using namespace glm;

namespace ZPG {

DynRotate::DynRotate(
    f32 startingRotationDeg, 
    f32 rotationSpeedDeg, 
    const v3& rotationAxis)
: m_CurrentRotationDeg(startingRotationDeg)
, m_RotationSpeedDeg(rotationSpeedDeg)
, m_RotationAxis(glm::normalize(rotationAxis)) {
    ComputeMatrix();
}

void DynRotate::Update(Timestep& ts) {
    m_CurrentRotationDeg += m_RotationSpeedDeg * ts.AsSeconds();
    ComputeMatrix();
}

void DynRotate::ComputeMatrix() {
    m4 R = glm::rotate(m4(1.0f), glm::radians(m_CurrentRotationDeg), m_RotationAxis);
    m_Matrix = R;
}

}
