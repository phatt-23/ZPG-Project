#include "DynScale.h"
#include <glm/gtc/matrix_transform.hpp>

#include "Core/Timestep.h"

namespace ZPG {

DynScale::DynScale(
    const v3& startingScale,
    const v3& growth,
    const v3& minScale,
    const v3& maxScale)
: m_CurrentScale(startingScale)
, m_Growth(growth)
, m_MinScale(minScale)
, m_MaxScale(maxScale) {
    ComputeMatrix();
}

void DynScale::Update(Timestep& ts) {
    glm::vec3 newScale = m_CurrentScale + (m_Growth * ts.AsSeconds());
    m_CurrentScale = glm::clamp(newScale, m_MinScale, m_MaxScale);
    ComputeMatrix();
}

void DynScale::ComputeMatrix() {
    m_Matrix = glm::scale(glm::mat4(1.0f), m_CurrentScale);
}


}
