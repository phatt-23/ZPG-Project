#include "PulseScaleTransform.h"

namespace ZPG {

PulseScaleTransform::PulseScaleTransform(
        const glm::vec3& startingScale, 
        const glm::vec3& growth, 
        const glm::vec3& minScale, 
        const glm::vec3& maxScale) 
: DynScaleTransform(startingScale, growth, minScale, maxScale){
}

void PulseScaleTransform::Update(Timestep ts) {
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
