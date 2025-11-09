//
// Created by phatt on 10/23/25.
//

#include "DrawCommand.h"

#include <netinet/tcp.h>

#include "Profiling/Instrumentor.h"

namespace ZPG {


DrawCommand::DrawCommand(ShaderProgram* shaderProgram, Material* material, VertexArray* vao)
    : m_ShaderProgram(shaderProgram)
    , m_Material(material)
    , m_VAO(vao) 
    , m_DrawIndex(-1)
{
    ZPG_PROFILE_FUNCTION();
    // ComputeSortKey();
}

DrawCommand::DrawCommand(Material* material, VertexArray* vao)
    : m_ShaderProgram(nullptr)
    , m_Material(material)
    , m_VAO(vao) 
    , m_DrawIndex(-1) 
{
    ZPG_PROFILE_FUNCTION();
    // ComputeSortKey();
}

static u64 HashPointer(const void* ptr) {
    return std::hash<const void*>{}(ptr) & 0xFFFFF; // 20 bits mask
}

void DrawCommand::ComputeSortKey() {
    u64 shaderBits = HashPointer(m_ShaderProgram) & 0xFFFFF;
    u64 materialBits = HashPointer(m_Material) & 0xFFFFF;
    u64 vaoBits = HashPointer(m_VAO) & 0xFFFFF;

    m_SortKey = (shaderBits << 40) | (materialBits << 20) | vaoBits;
}

bool DrawCommand::DrawCommandComparator::operator()(const DrawCommand& a, const DrawCommand& b) const
{
    if (a.m_ShaderProgram != b.m_ShaderProgram) {
        return a.m_ShaderProgram < b.m_ShaderProgram;
    }

    if (a.m_Material != b.m_Material) {
        return a.m_Material < b.m_Material;
    }

    return a.m_VAO < b.m_VAO;
}

} // ZPG
