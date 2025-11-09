#include "PulseScale.h"

#include "Core/Timestep.h"
#include "Profiling/Instrumentor.h"

namespace ZPG {

PulseScale::PulseScale(
        const v3& startingScale,
        const v3& growth,
        const v3& minScale,
        const v3& maxScale)
: DynScale(startingScale, growth, minScale, maxScale){
    ZPG_PROFILE_FUNCTION();
}

void PulseScale::Update(Timestep& ts) {
    ZPG_PROFILE_FUNCTION();

    glm::vec3 newScale = glm::clamp(
        m_CurrentScale + (m_Growth * ts.AsSeconds()),
        m_MinScale,
        m_MaxScale);

    if (newScale == m_MaxScale || newScale == m_MinScale) {
        m_Growth = -m_Growth;
    }

    m_CurrentScale = newScale;
    ComputeMatrix();
}

}
