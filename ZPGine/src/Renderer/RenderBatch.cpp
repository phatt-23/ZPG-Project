//
// Created by phatt on 10/23/25.
//

#include "RenderBatch.h"

namespace ZPG {


RenderBatch::RenderBatch(u32 batchCapacity)
: m_BatchCapacity(batchCapacity) {
    m_DrawCommands.reserve(m_BatchCapacity);
}

RenderBatch::~RenderBatch() {
}

void RenderBatch::AddCommand(const DrawCommand& command) {
    m_DrawCommands.push_back(command);
}

void RenderBatch::Reset() {
    m_DrawCommands.clear();
}

void RenderBatch::SortCommands() {
    std::ranges::sort(m_DrawCommands.begin(), m_DrawCommands.end(), DrawCommand::DrawCommandComparator());
}

const std::vector<DrawCommand>& RenderBatch::GetDrawCommands() const {
    return m_DrawCommands;
}

u32 RenderBatch::GetBatchSize() const {
    return m_DrawCommands.size();
}

u32 RenderBatch::GetBatchCapacity() const {
    return m_BatchCapacity;
}

}
