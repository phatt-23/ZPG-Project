//
// Created by phatt on 10/18/25.
//
#include "PositionComponent.h"

namespace ZPG {


PositionComponent::PositionComponent(const v3& position)
: m_Position(position) {
}

void PositionComponent::Set(const v3& position) {
    m_Position = position;
}
const v3& PositionComponent::Get() const {
    return m_Position;
}


}
