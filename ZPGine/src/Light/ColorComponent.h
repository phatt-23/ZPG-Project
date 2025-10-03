#pragma once
#include <glm/glm.hpp>

namespace ZPG {

class ColorComponent {
public:
    ColorComponent(const glm::vec4& color = glm::vec4(1.f, 1.f, 1.f, 1.f)) 
    : m_Color(color) {
    }
    void SetColor(const glm::vec4& color) { m_Color = color; }
    const glm::vec4& GetColor() const { return m_Color; }
private:
    glm::vec4 m_Color;
};
    
}
