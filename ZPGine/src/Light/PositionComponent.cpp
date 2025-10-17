//
// Created by phatt on 10/18/25.
//
#include "PositionComponent.h"

namespace ZPG {


PositionComponent::PositionComponent(const v3& position)
: m_Position(position) {
}

void PositionComponent::SetPosition(const v3& position) {
    m_Position = position;
}
const v3& PositionComponent::GetPosition() const {
    return m_Position;
}


}
