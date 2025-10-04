#pragma once
#include "Model/Vertex.h"
#include "Renderer/VertexArray.h"

namespace ZPG {

class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<u32>& indices);
    Mesh(const Ref<VertexArray>& vertexArray);
    void Bind();
    void Unbind();
private:
    Ref<VertexArray> m_VAO;
};

}
