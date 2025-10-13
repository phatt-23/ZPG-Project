#include "PulseScale.h"

namespace ZPG {

PulseScale::PulseScale(
        const glm::vec3& startingScale, 
        const glm::vec3& growth, 
        const glm::vec3& minScale, 
        const glm::vec3& maxScale) 
: DynScale(startingScale, growth, minScale, maxScale){
}

void PulseScale::Update(Timestep ts) {
    glm::vec3 newScale = glm::clamp(m_CurrentScale + (m_Growth * ts.AsSeconds()),
                                m_MinScale, 
                                m_MaxScale);

    if (newScale == m_MaxScale || newScale == m_MinScale) {
        m_Growth = -m_Growth;
    }

    m_CurrentScale = newScale;
    ComputeMatrix();
}

}
