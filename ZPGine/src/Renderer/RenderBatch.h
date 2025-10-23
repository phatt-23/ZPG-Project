//
// Created by phatt on 10/23/25.
//

#ifndef WORKSPACE_RENDER_BATCH_H
#define WORKSPACE_RENDER_BATCH_H
#include "DrawCommand.h"

namespace ZPG {


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

    void AddCommand(const DrawCommand& command);
    void SortCommands();
    void Reset();

    const std::vector<DrawCommand>& GetDrawCommands() const;

    u32 GetBatchSize() const;
    u32 GetBatchCapacity() const;

private:
    std::vector<DrawCommand> m_DrawCommands;
    u32 m_BatchCapacity;
};

} // ZOG

#endif //WORKSPACE_RENDER_BATCH_H