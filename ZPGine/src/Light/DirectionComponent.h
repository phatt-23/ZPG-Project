#pragma once
#include <glm/glm.hpp>

namespace ZPG {

class DirectionComponent {
public:
    DirectionComponent(const glm::vec3& direction = glm::vec3(0.f, 0.f, -1.f)) 
    : m_Direction(direction) {
    }
    void SetDirection(const glm::vec3& direction) { m_Direction = glm::normalize(direction); }
    const glm::vec3& GetDirection() const { return m_Direction; }
private:
    glm::vec3 m_Direction;
};
    
}
