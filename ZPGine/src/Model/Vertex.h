#pragma once
#include "Renderer/BufferLayout.h"
#include <glm/glm.hpp>

namespace ZPG {

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexUV;

    Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texUV) 
    : Position(position) 
    , Normal(normal)
    , TexUV(texUV) { 
    }

    static BufferLayout GetLayout() {
        return BufferLayout({
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float3, "a_Normal"},
            {ShaderDataType::Float2, "a_TexUV"},
        });
    }
};

}
