//
// Created by phatt on 10/23/25.
//

#include "DrawCommand.h"

namespace ZPG {

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