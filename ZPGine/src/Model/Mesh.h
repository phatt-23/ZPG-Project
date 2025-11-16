#pragma once

namespace ZPG {

class Vertex;
class VertexArray;
class Material;

class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices, 
        const std::vector<u32>& indices, 
        const m4& localTransform = m4(1.0f));

    Mesh(const ref<VertexArray>& vertexArray, 
         const m4& localTransform = m4(1.0f));

    void SetMaterial(const ref<Material>& material);

    // void Bind();
    // void Unbind();

    const m4& GetLocalTransform() const { return m_LocalTransform; }
    const ref<VertexArray>& GetVertexArray() const { return m_VAO; }
    const ref<Material>& GetMaterial() const { return m_Material; }

    u32 GetID() const { return m_ID; }

    static ref<Mesh> Create(
        const std::vector<Vertex>& vertices,
        const std::vector<u32>& indices, 
        const m4& localTransform = m4(1.0f));

    static ref<Mesh> Create(
        const ref<VertexArray>& vertexArray,
        const m4& localTransform = m4(1.0f));

    static ref<Mesh> Create(
        const ref<VertexArray>& vertexArray,
        const ref<Material>& material,
        const m4& localTransform = m4(1.0f));

private:
    u32 m_ID = -1;
    ref<VertexArray> m_VAO;
    m4 m_LocalTransform;
    ref<Material> m_Material = nullptr;

    inline static u32 s_NextID = 0;
};

}
