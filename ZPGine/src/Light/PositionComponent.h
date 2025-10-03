#pragma once
#include <glm/glm.hpp>

namespace ZPG {

class PositionComponent {
public:
    PositionComponent(const glm::vec3& position = glm::vec3(0.f, 0.f, -1.f)) 
    : m_Position(position) {
    }
    void SetPosition(const glm::vec3& position) { m_Position = position; }
    const glm::vec3& GetPosition() const { return m_Position; }
private:
    glm::vec3 m_Position;
};
    
}
