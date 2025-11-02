//
// Created by phatt on 10/23/25.
//

#ifndef WORKSPACE_DRAW_COMMAND_H
#define WORKSPACE_DRAW_COMMAND_H

namespace ZPG {

class ShaderProgram;
class Material;
class VertexArray;
class Transform;

struct DrawCommand {
    DrawCommand(ShaderProgram* shaderProgram, Material* material, VertexArray* vao);

    ShaderProgram* m_ShaderProgram;
    Material* m_Material;
    VertexArray* m_VAO;

    u32 m_DrawIndex;

    /**
     * These are not stored here directly, rather they are accessed by the m_DrawIndex in the RenderBatch.
     *      m4 m_Transform;
     *      i32 m_EntityID;
     */



    struct DrawCommandComparator {
        bool operator() (const DrawCommand& a, const DrawCommand& b) const;
    };
};

} // ZPG

#endif //WORKSPACE_DRAW_COMMAND_H
