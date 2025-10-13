#include "Mesh.h"
#include "Core/ResourceManager.h"


namespace ZPG {

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<u32>& indices, const glm::mat4& localTransform)
: m_VAO(VertexArray::Create())
, m_LocalTransform(localTransform) {
    u32 vboSize = vertices.size() * sizeof(vertices[0]);
    u32 iboCount = indices.size();
    
    Ref<VertexBuffer> vbo = VertexBuffer::Create(vertices.data(), vboSize);
    vbo->SetLayout(Vertex::GetLayout());

    Ref<IndexBuffer> ibo = IndexBuffer::Create(indices.data(), iboCount);
    
    m_VAO->AddVertexBuffer(vbo);
    m_VAO->SetIndexBuffer(ibo);

    auto defaultMaterial = ResourceManager::GetGlobal().GetMaterial("null");
    SetMaterial(defaultMaterial);
}

Mesh::Mesh(const Ref<VertexArray>& vertexArray, const glm::mat4& localTransform) 
: m_VAO(vertexArray)
, m_LocalTransform(localTransform) {
    auto defaultMaterial = ResourceManager::GetGlobal().GetMaterial("null");
    SetMaterial(defaultMaterial);
}

void Mesh::Bind() {
    m_VAO->Bind();
}

void Mesh::Unbind() {
    m_VAO->Unbind();
}

void Mesh::SetMaterial(const Ref<Material>& material) {
    m_Material = material;
}


Ref<Mesh> Mesh::Create(
    const std::vector<Vertex>& vertices, 
    const std::vector<u32>& indices, 
    const glm::mat4& localTransform
) {
    Mesh* mesh = new Mesh(vertices, indices, localTransform);
    return CreateRef(mesh);
}

Ref<Mesh> Mesh::Create(const Ref<VertexArray>& vertexArray, const glm::mat4& localTransform) {
    Mesh* mesh = new Mesh(vertexArray, localTransform);
    return CreateRef(mesh);
}

}
