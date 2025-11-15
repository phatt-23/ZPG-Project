//
// Created by phatt on 10/23/25.
//

#ifndef WORKSPACE_RENDER_BATCH_H
#define WORKSPACE_RENDER_BATCH_H
#include "DrawCommand.h"

namespace ZPG {

    class Mesh;
    struct VertexArrayGroup;
    struct MaterialGroup;
    struct ShaderProgramGroup;

    class RenderBatch {
    public:
        RenderBatch(u32 batchCapacity);
        ~RenderBatch();

        void AddCommand(DrawCommand& command, const m4& transform, i32 entityID = -1);
        void SubmitMesh(const Mesh& mesh, const m4& transform, i32 entityID = -1);

        void SortCommands();
        void Reset();

        const std::vector<DrawCommand>& GetDrawCommands() const;

        u32 GetBatchSize() const;
        u32 GetBatchCapacity() const;
        bool IsFull() const;

        void BuildGroups();

        const std::vector<ShaderProgramGroup>& GetShaderProgramGroups() const;
        const std::vector<MaterialGroup>& GetMaterialGroups() const;
        const std::vector<VertexArrayGroup>& GetVertexArrayGroups() const;
        
        const std::vector<m4>& GetTransforms() const;
        const m4& GetTransform(u32 transformIndex) const;

        const std::vector<i32>& GetEntityIDs() const;

    private:
        u32 m_BatchCapacity;

        std::vector<DrawCommand> m_DrawCommands;
        std::vector<m4> m_Transforms;
        std::vector<i32> m_EntityIDs;

        // indexers
        std::vector<ShaderProgramGroup> m_ShaderProgramGroups;
        std::vector<MaterialGroup> m_MaterialGroups;
        std::vector<VertexArrayGroup> m_VertexArrayGroups;


        // cache
        std::vector<m4> m_SortedTransforms;
        std::vector<i32> m_SortedEntityIDs;

        bool m_NeedsSorting;
    };

} // ZPG

#endif //WORKSPACE_RENDER_BATCH_H
