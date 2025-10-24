//
// Created by phatt on 10/24/25.
//

#ifndef WORKSPACE_RENDER_GROUPS_H
#define WORKSPACE_RENDER_GROUPS_H
#include <glm/fwd.hpp>

#include "Core/Core.h"

namespace ZPG {

class Material;
class ShaderProgram;
class VertexArray;

/**
 * These act as indexers into a flat draw calls vector.
 * It allows hierarchical rendering without the hierarchical cost of, for example, trees or maps and so on.
 */

struct ShaderProgramGroup {
    ShaderProgram* m_ShaderProgram;
    u32 m_MaterialStart;
    u32 m_MaterialCount;
};

struct MaterialGroup {
    Material* m_Material;
    u32 m_VertexArrayStart;
    u32 m_VertexArrayCount;
};

struct VertexArrayGroup {
    VertexArray* m_VertexArray;
    u32 m_Start;
    u32 m_Count;
};

}

#endif //WORKSPACE_RENDER_GROUPS_H
