//
// Created by phatt on 10/18/25.
//

#include "Vertex.h"
#include "Buffer/BufferLayout.h"

namespace ZPG {


Vertex::Vertex(const v3& position, const v3& normal, const v2& texUV, const v3& tangent)
: Position(position)
, Normal(normal)
, TexUV(texUV)
, Tangent(tangent) {
}

BufferLayout Vertex::GetLayout() {
    return BufferLayout({
        {ShaderDataType::Float3, "a_Position"},
        {ShaderDataType::Float3, "a_Normal"},
        {ShaderDataType::Float2, "a_TexUV"},
        {ShaderDataType::Float3, "a_Tangent"},
    });
}

}