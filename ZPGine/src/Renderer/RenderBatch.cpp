//
// Created by phatt on 10/23/25.
//

#include "RenderBatch.h"
#include "RenderGroups.h"

namespace ZPG {


RenderBatch::RenderBatch(u32 batchCapacity)
: m_BatchCapacity(batchCapacity) {
    m_DrawCommands.reserve(m_BatchCapacity);

    m_ShaderProgramGroups.reserve(m_BatchCapacity);
    m_MaterialGroups.reserve(m_BatchCapacity);
    m_VertexArrayGroups.reserve(m_BatchCapacity);
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

void RenderBatch::BuildGroups() {
    // clear old data
    m_ShaderProgramGroups.clear();
    m_MaterialGroups.clear();
    m_VertexArrayGroups.clear();

    // nothing to do
    if (m_DrawCommands.empty()) {
        return;
    }

    // presort draw commands
    SortCommands();

    // reserve upfront
    m_ShaderProgramGroups.reserve(64);
    m_MaterialGroups.reserve(256);
    m_VertexArrayGroups.reserve(1024);

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

        if (newVAO) {
            // finish previous VAO
            m_VertexArrayGroups.push_back({
                currVAO,             
                vaoTransformStart,  // start from i-th draw command
                vaoTransformCount,  // up until (i + count)-th command
            }); 

            vaoTransformStart = i;  // the i-th draw command
            vaoTransformCount = 0;

            materialVaoCount += 1;  // material gets one more vao group

            currVAO = command.m_VAO;
        }

        if (newMaterial) {
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

}
