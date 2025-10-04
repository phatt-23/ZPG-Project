#include "Mesh.h"

namespace ZPG {

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<u32>& indices, const glm::mat4& localTransform)
: m_VAO(VertexArray::Create())
, m_LocalTransform(localTransform) {
        u32 vboSize = vertices.size() * sizeof(vertices[0]);
        u32 iboSize = indices.size() * sizeof(indices[0]);
        
        Ref<VertexBuffer> vbo = VertexBuffer::Create((f32*)vertices.data(), vboSize);
        Ref<IndexBuffer> ibo = IndexBuffer::Create(indices.data(), iboSize);
        
        m_VAO->AddVertexBuffer(vbo);
    m_VAO->SetIndexBuffer(ibo);
}

Mesh::Mesh(const Ref<VertexArray>& vertexArray, const glm::mat4& localTransform) 
: m_VAO(vertexArray)
, m_LocalTransform(localTransform) {
}

void Mesh::Bind() {
    m_VAO->Bind();
}

void Mesh::Unbind() {
    m_VAO->Unbind();
}

}
