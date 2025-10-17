#pragma once
#include "Renderer/BufferLayout.h"
#include <glm/glm.hpp>

namespace ZPG {

struct Vertex {
    v3 Position;
    v3 Normal;
    v2 TexUV;
    v3 Tangent;

    Vertex(const v3& position, const v3& normal, const v2& texUV, const v3& tangent)
    : Position(position) 
    , Normal(normal)
    , TexUV(texUV)
    , Tangent(tangent) {
    }

    static BufferLayout GetLayout() {
        return BufferLayout({
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float3, "a_Normal"},
            {ShaderDataType::Float2, "a_TexUV"},
            {ShaderDataType::Float3, "a_Tangent"},
        });
    }
};

}
