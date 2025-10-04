#pragma once
#include "Renderer/BufferLayout.h"
#include <glm/glm.hpp>

namespace ZPG {

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexUV;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;

    Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texUV, 
           const glm::vec3& tangent, const glm::vec3& bitangent) 
    : Position(position) 
    , Normal(normal)
    , TexUV(texUV)
    , Tangent(tangent)
    , Bitangent(bitangent) { 
    }

    static BufferLayout GetLayout() {
        return BufferLayout({
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float3, "a_Normal"},
            {ShaderDataType::Float2, "a_TexUV"},
            {ShaderDataType::Float3, "a_Tangent"},
            {ShaderDataType::Float3, "a_Bitangent"},
        });
    }
};

}
