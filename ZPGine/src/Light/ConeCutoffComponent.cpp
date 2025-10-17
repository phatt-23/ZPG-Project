//
// Created by phatt on 10/17/25.
//
#include "ConeCutoffComponent.h"

namespace ZPG {

ConeCutoffComponent::ConeCutoffComponent(f32 innerCutoff, f32 outerCutoff)
: m_InnerCutoff(glm::radians(innerCutoff))
, m_OuterCutoff(glm::radians(outerCutoff)) {
}
f32 ConeCutoffComponent::GetInnerCutoff() const {
    return m_InnerCutoff;
}
void ConeCutoffComponent::SetInnerCutoff(f32 innerCutoff) {
    m_InnerCutoff = innerCutoff;
}

void ConeCutoffComponent::SetOuterCutoff(f32 outerCutoff) {
    m_OuterCutoff = outerCutoff;
}
f32 ConeCutoffComponent::GetOuterCutoff() const {
    return m_OuterCutoff;
}


}
