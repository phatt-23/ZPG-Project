#pragma once

#include "Core/Core.h"
#include <glm/glm.hpp>

namespace ZPG {

class ConeCutoffComponent {
public:
    ConeCutoffComponent(f32 innerCutoff, f32 outerCutoff)
    : m_InnerCutoff(glm::radians(innerCutoff)), m_OuterCutoff(glm::radians(outerCutoff)) {
    }
    f32 GetInnerCutoff() const { return m_InnerCutoff; }
    void SetInnerCutoff(f32 innerCutoff) { m_InnerCutoff = innerCutoff; }
    f32 GetOuterCutoff() const { return m_OuterCutoff; }
    void SetOuterCutoff(f32 outerCutoff) { m_OuterCutoff = outerCutoff; }
private:
    f32 m_InnerCutoff;  // in radians
    f32 m_OuterCutoff;
};

}
