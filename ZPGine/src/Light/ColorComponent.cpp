//
// Created by phatt on 10/17/25.
//
#include "ColorComponent.h"

namespace ZPG {

ColorComponent::ColorComponent(const v4& color)
: m_Color(color) {
}

void ColorComponent::SetColor(const v4& color) {
    m_Color = color;
}
const v4& ColorComponent::GetColor() const {
    return m_Color;
}

}
