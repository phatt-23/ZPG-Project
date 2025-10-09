#pragma once
#include "Model/Vertex.h"
#include "Renderer/VertexArray.h"
#include "Material.h"

namespace ZPG {

class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices, 
        const std::vector<u32>& indices, 
        const glm::mat4& localTransform = glm::mat4(1.0f));

    Mesh(const Ref<VertexArray>& vertexArray, 
         const glm::mat4& localTransform = glm::mat4(1.0f));

    void SetMaterial(const Ref<Material>& material);

    void Bind();
    void Unbind();

    const glm::mat4& GetLocalTransform() const { return m_LocalTransform; }
    const Ref<VertexArray>& GetVertexArray() const { return m_VAO; }
    const Ref<Material>& GetMaterial() const { return m_Material; }
private:
    Ref<VertexArray> m_VAO;
    glm::mat4 m_LocalTransform;
    Ref<Material> m_Material = nullptr;
};

}
