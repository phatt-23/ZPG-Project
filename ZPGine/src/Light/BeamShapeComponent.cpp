//
// Created by phatt on 10/17/25.
//
#include "BeamShapeComponent.h"

namespace ZPG {

BeamShapeComponent::BeamShapeComponent(f32 size, f32 blend) {
    SetSize(size);
    SetBlend(blend);
}
f32 BeamShapeComponent::GetSize() const { 
    return m_Size; 
}
f32 BeamShapeComponent::GetBlend() const { 
    return m_Blend; 
}
void BeamShapeComponent::SetSize(f32 size) {
    m_Size = size;
    m_SizeCos = glm::cos(glm::radians(m_Size));
}
void BeamShapeComponent::SetBlend(f32 blend) { 
    m_Blend = blend;
    m_BlendCos = glm::cos(glm::radians(m_Blend * m_Size));
}

}
