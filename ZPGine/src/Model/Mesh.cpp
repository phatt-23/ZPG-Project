#include "Mesh.h"

#include "Buffer/VertexBuffer.h"
#include "Buffer/VertexArray.h"
#include "Buffer/BufferLayout.h"
#include "Resource/CommonResources.h"
#include "Resource/ResourceManager.h"
#include "Vertex.h"


namespace ZPG {

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<u32>& indices, const glm::mat4& localTransform)
: m_VAO(VertexArray::Create())
, m_LocalTransform(localTransform) {
    u32 vboSize = vertices.size() * sizeof(vertices[0]);
    u32 iboCount = indices.size();
    
    ref<VertexBuffer> vbo = VertexBuffer::Create(vertices.data(), vboSize);
    vbo->SetLayout(Vertex::GetLayout());

    ref<IndexBuffer> ibo = IndexBuffer::Create(indices.data(), iboCount);
    
    m_VAO->AddVertexBuffer(vbo);
    m_VAO->SetIndexBuffer(ibo);

    auto defaultMaterial = ResourceManager::GetGlobal().GetMaterial(CommonResources::NULL_MATERIAL);
    SetMaterial(defaultMaterial);
}

Mesh::Mesh(const ref<VertexArray>& vertexArray, const glm::mat4& localTransform) 
: m_VAO(vertexArray)
, m_LocalTransform(localTransform) {
    auto defaultMaterial = ResourceManager::GetGlobal().GetMaterial(CommonResources::NULL_MATERIAL);
    SetMaterial(defaultMaterial);
}

void Mesh::Bind() {
    m_VAO->Bind();
}

void Mesh::Unbind() {
    m_VAO->Unbind();
}

void Mesh::SetMaterial(const ref<Material>& material) {
    m_Material = material;
}


ref<Mesh> Mesh::Create(
    const std::vector<Vertex>& vertices, 
    const std::vector<u32>& indices, 
    const glm::mat4& localTransform
) {
    Mesh* mesh = new Mesh(vertices, indices, localTransform);
    return MakeRef(mesh);
}

ref<Mesh> Mesh::Create(const ref<VertexArray>& vertexArray, const glm::mat4& localTransform) {
    Mesh* mesh = new Mesh(vertexArray, localTransform);
    return MakeRef(mesh);
}


ref<Mesh> Mesh::Create(
    const ref<VertexArray>& vertexArray,
    const ref<Material>& material,
    const m4& localTransform
) {
    Mesh* mesh = new Mesh(vertexArray, localTransform);
    mesh->SetMaterial(material);
    return MakeRef(mesh);
}

}
