//
// Created by phatt on 10/23/25.
//

#ifndef WORKSPACE_RENDER_BATCH_H
#define WORKSPACE_RENDER_BATCH_H
#include "DrawCommand.h"

namespace ZPG {
struct VertexArrayGroup;
struct MaterialGroup;
struct ShaderProgramGroup;


/**
    For batching draw calls.
    After exceeding some threshold (batch size),
    the batch has to be flushed and reset.

    I want to achieve this:
     SELECT *
     FROM Batch
     GROUP BY (ShaderProgram, Material, VAO, Transform)
*/

class RenderBatch {
public:
    RenderBatch(u32 batchCapacity);
    ~RenderBatch();

    void AddCommand(DrawCommand& command, const m4& transform, i32 entityID = -1);
    void SortCommands();
    void Reset();

    const std::vector<DrawCommand>& GetDrawCommands() const;

    u32 GetBatchSize() const;
    u32 GetBatchCapacity() const;

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

    bool m_NeedsSorting;
};

} // ZPG

#endif //WORKSPACE_RENDER_BATCH_H
