#pragma once

#include "Core/Core.h"

namespace ZPG {

class ConeCutoffComponent {
public:
    ConeCutoffComponent(f32 innerCutoff, f32 outerCutoff)
    : m_InnerCutoff(innerCutoff), m_OuterCutoff(outerCutoff) {
    }
    f32 GetInnerCutoff() const { return m_InnerCutoff; }
    void SetInnerCutoff(f32 innerCutoff) { m_InnerCutoff = innerCutoff; }
    f32 GetOuterCutoff() const { return m_OuterCutoff; }
    void SetOuterCutoff(f32 outerCutoff) { m_OuterCutoff = outerCutoff; }
private:
    f32 m_InnerCutoff;
    f32 m_OuterCutoff;
};

}
