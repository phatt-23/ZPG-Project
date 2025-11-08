//
// Created by phatt on 10/23/25.
//

#include "DrawCommand.h"

namespace ZPG {


DrawCommand::DrawCommand(ShaderProgram* shaderProgram, Material* material, VertexArray* vao)
    : m_ShaderProgram(shaderProgram)
    , m_Material(material)
    , m_VAO(vao) 
    , m_DrawIndex(-1) {
}

DrawCommand::DrawCommand(Material* material, VertexArray* vao)
    : m_ShaderProgram(nullptr)
    , m_Material(material)
    , m_VAO(vao) 
    , m_DrawIndex(-1) 
{
}

bool DrawCommand::DrawCommandComparator::operator()(const DrawCommand& a, const DrawCommand& b) const {
    if (a.m_ShaderProgram != b.m_ShaderProgram) {
        return a.m_ShaderProgram < b.m_ShaderProgram;
    }

    if (a.m_Material != b.m_Material) {
        return a.m_Material < b.m_Material;
    }

    return a.m_VAO < b.m_VAO;
}

} // ZPG
