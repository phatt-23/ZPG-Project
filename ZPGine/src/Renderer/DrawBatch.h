//
// Created by phatt on 10/23/25.
//

#ifndef WORKSPACE_RENDER_BATCH_H
#define WORKSPACE_RENDER_BATCH_H

namespace ZPG {

    class Mesh;
    class ShaderProgram;
    class Material;
    class VertexArray;

    // key
    struct DrawBatchKey
    {
        u32 MeshId;
    
        bool operator==(DrawBatchKey const& other) const 
        {
            return MeshId == other.MeshId;
        }
    };

    // value
    struct DrawBatch
    {
        Mesh* mesh;
        vec<i32> entityIDs;
        vec<m4> transforms;

        u32 GetSize() const { return entityIDs.size(); }
    };
} 

namespace std
{
    // hasher
    template<>
    struct hash<ZPG::DrawBatchKey>
    {
        size_t operator()(const ZPG::DrawBatchKey& key) const noexcept
        {
            return std::hash<ZPG::u32>()(key.MeshId);
        }
    };
}

#endif 
