#include "Mesh.h"

#include "Buffer/BufferLayout.h"
#include "Buffer/VertexBuffer.h"
#include "Buffer/VertexArray.h"
#include "Culling/Volume.h"
#include "Material/Material.h"
#include "Resource/CommonResources.h"
#include "Resource/ResourceManager.h"
#include "Vertex.h"
#include "Profiling/Instrumentor.h"


namespace ZPG 
{
    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<u32>& indices, const glm::mat4& localTransform)
        : m_ID(s_NextID++)
        , m_VAO(VertexArray::Create())
        , m_LocalTransform(localTransform)
    {
        ZPG_PROFILE_FUNCTION();

        u32 vboSize = vertices.size() * sizeof(vertices[0]);
        u32 iboCount = indices.size();
        
        ref<VertexBuffer> vbo = VertexBuffer::Create(vertices.data(), vboSize, Vertex::GetLayout());

        ref<IndexBuffer> ibo = IndexBuffer::Create(indices.data(), iboCount);
        
        m_VAO->AddVertexBuffer(vbo);
        m_VAO->SetIndexBuffer(ibo);

        auto defaultMaterial = ResourceManager::GetGlobal().GetMaterial(CommonResources::NULL_MATERIAL);
        SetMaterial(defaultMaterial);

        v3 minBound(std::numeric_limits<f32>::max());
        v3 maxBound(std::numeric_limits<f32>::min());
        for (const Vertex& v : vertices)
        {
            minBound = glm::min(v.Position, minBound);
            maxBound = glm::max(v.Position, maxBound);
        }

        m_Volume = MakeRef(new AABB(minBound, maxBound));

        ZPG_CORE_ASSERT(m_VAO != nullptr);
        ZPG_CORE_ASSERT(m_Material != nullptr);
        ZPG_CORE_ASSERT(m_Volume != nullptr);
    }

    void Mesh::SetMaterial(const ref<Material>& material) 
    {
        ZPG_PROFILE_FUNCTION();
        ZPG_CORE_ASSERT(material != nullptr, "Material cannot be null!");
        m_Material = material;
    }

    ref<Mesh> Mesh::Create(const std::vector<Vertex>& vertices, const std::vector<u32>& indices, const glm::mat4& localTransform) 
    {
        ZPG_PROFILE_FUNCTION();
        Mesh* mesh = new Mesh(vertices, indices, localTransform);
        return MakeRef(mesh);
    }
}
