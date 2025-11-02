//
// Created by phatt on 10/23/25.
//

#include "RenderBatch.h"
#include "Debug/Asserter.h"
#include "RenderGroups.h"

namespace ZPG {


RenderBatch::RenderBatch(u32 batchCapacity)
: m_BatchCapacity(batchCapacity)
, m_NeedsSorting(false) {
    m_DrawCommands.reserve(m_BatchCapacity);
    m_Transforms.reserve(m_BatchCapacity);

    // reserve upfront
    // since there are at most m_BatchCapacity draw commands,
    // we need at most m_BatchCapacity of every group
    m_ShaderProgramGroups.reserve(m_BatchCapacity);
    m_MaterialGroups.reserve(m_BatchCapacity);
    m_VertexArrayGroups.reserve(m_BatchCapacity);
}

RenderBatch::~RenderBatch() {
}

void RenderBatch::AddCommand(DrawCommand& command, const m4& transform, i32 entityID) {
    m_Transforms.push_back(transform);
    m_EntityIDs.push_back(glm::i32vec4(entityID, 1.0, 1.0, 1.0));

    command.m_DrawIndex = m_Transforms.size() - 1;

    m_DrawCommands.push_back(command);
    m_NeedsSorting = true;  // mark dirty
}

void RenderBatch::Reset() {
    m_DrawCommands.clear();
    m_Transforms.clear();
    m_EntityIDs.clear();
    m_NeedsSorting = false;
}

void RenderBatch::SortCommands() {
    m_NeedsSorting = false;

    std::ranges::sort(
        m_DrawCommands.begin(), 
        m_DrawCommands.end(), 
        DrawCommand::DrawCommandComparator());

    // after sorting, reorder transforms to match
    std::vector<m4> sortedTransforms(m_DrawCommands.size());
    std::vector<glm::i32vec4> sortedEntityIDs(m_DrawCommands.size());

    for (size_t i = 0; i < m_DrawCommands.size(); i++)
    {
        sortedTransforms[i] = m_Transforms[m_DrawCommands[i].m_DrawIndex];
        sortedEntityIDs[i] = m_EntityIDs[m_DrawCommands[i].m_DrawIndex];
    }

    m_Transforms = std::move(sortedTransforms);
    m_EntityIDs = std::move(sortedEntityIDs);

    // update each command’s transform index to reflect the new sorted order
    for (size_t i = 0; i < m_DrawCommands.size(); i++)
    {
        m_DrawCommands[i].m_DrawIndex = i;
    }
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

void RenderBatch::BuildGroups() {
    if (m_DrawCommands.empty()) {
        return;
    }

    // clear old data (only set size to 0)
    m_ShaderProgramGroups.resize(0);
    m_MaterialGroups.resize(0);
    m_VertexArrayGroups.resize(0);

    // presort draw commands
    SortCommands();

    // current assets being used
    ShaderProgram*  currShaderProgram   = m_DrawCommands[0].m_ShaderProgram;
    Material*       currMaterial        = m_DrawCommands[0].m_Material;
    VertexArray*    currVAO             = m_DrawCommands[0].m_VAO;

    // indices - start and count
    u32 shaderProgramMaterialStart = 0;
    u32 shaderProgramMaterialCount = 0;

    u32 materialVaoStart = 0;
    u32 materialVaoCount = 0;

    u32 vaoTransformStart = 0;
    u32 vaoTransformCount = 0;

    const u32 drawCommandCount = m_DrawCommands.size();

    // build draw groups
    for (u32 i = 0; i < drawCommandCount; i++) {
        const DrawCommand& command = m_DrawCommands[i];

        bool newShaderProgram   = currShaderProgram != command.m_ShaderProgram;
        bool newMaterial        = currMaterial != command.m_Material;
        bool newVAO             = currVAO != command.m_VAO;

        if (newVAO || newMaterial || newShaderProgram) {
            // finish previous VAO
            m_VertexArrayGroups.push_back({
                currVAO,             
                vaoTransformStart,  // start from i-th draw command
                vaoTransformCount,  // up until (i + count)-th command
            }); 

            vaoTransformStart = i;  // begins at the i-th draw command
            vaoTransformCount = 0;

            materialVaoCount += 1;  // material gets one more vao group

            currVAO = command.m_VAO;
        }

        if (newMaterial || newShaderProgram) {
            // finish previous Material
           
            m_MaterialGroups.push_back({
                currMaterial,
                materialVaoStart,  // material
                materialVaoCount,
            });

            materialVaoStart += materialVaoCount;  // new material must start after this material group's range of vao groups
            materialVaoCount = 0;

            shaderProgramMaterialCount += 1;  // shader program gets one more material group

            currMaterial = command.m_Material;
        }

        if (newShaderProgram) {
            // finish previous ShaderProgram 
            m_ShaderProgramGroups.push_back({
                currShaderProgram, 
                shaderProgramMaterialStart,
                shaderProgramMaterialCount,
            });

            shaderProgramMaterialStart += shaderProgramMaterialCount;  // a new shader program must start after this shader program's range of material groups
            shaderProgramMaterialCount = 0;

            currShaderProgram = command.m_ShaderProgram;
        }

        // count this transform into this the current vao group
        vaoTransformCount++;
    }

    // push whichever groups are still active (the final ones)
    // final VAO
    m_VertexArrayGroups.push_back({
        currVAO,
        vaoTransformStart,
        vaoTransformCount
    });

    materialVaoCount += 1; // the last material has one more VAO group
    shaderProgramMaterialCount += 1; // the last shader program has one more material group

    // final material
    m_MaterialGroups.push_back({
        currMaterial,
        materialVaoStart,
        materialVaoCount
    });

    // final shader program
    m_ShaderProgramGroups.push_back({
        currShaderProgram,
        shaderProgramMaterialStart,
        shaderProgramMaterialCount
    });
}

const std::vector<ShaderProgramGroup>& RenderBatch::GetShaderProgramGroups() const {
    return m_ShaderProgramGroups;
}

const std::vector<MaterialGroup>& RenderBatch::GetMaterialGroups() const {
    return m_MaterialGroups;
}

const std::vector<VertexArrayGroup>& RenderBatch::GetVertexArrayGroups() const {
    return m_VertexArrayGroups;
}

const m4& RenderBatch::GetTransform(u32 transformIndex) const {
    ZPG_CORE_ASSERT(transformIndex < m_Transforms.size(), "Cannot index Transforms with the size of {} at position {}", m_Transforms.size(), transformIndex);
    return m_Transforms[transformIndex];
}
const std::vector<m4>& RenderBatch::GetTransforms() const {
    return m_Transforms;
}

const std::vector<glm::i32vec4>& RenderBatch::GetEntityIDs() const {
    return m_EntityIDs;
}

}
