//
// Created by phatt on 10/17/25.
//
#include "DirectionComponent.h"

namespace ZPG {


DirectionComponent::DirectionComponent(const v3& direction)
: m_Direction(direction) {
}
void DirectionComponent::SetDirection(const v3& direction) {
    m_Direction = glm::normalize(direction);
}
const v3& DirectionComponent::GetDirection() const {
    return m_Direction;
}


}
