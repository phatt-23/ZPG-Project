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
    DrawCommand() = default;
    DrawCommand(ShaderProgram* shaderProgram, Material* material, VertexArray* vao);
    DrawCommand(Material* material, VertexArray* vao);

    // u64 SortKey = 0;
    ShaderProgram* m_ShaderProgram = nullptr;
    Material* m_Material = nullptr;
    VertexArray* m_VAO = nullptr;
    u32 m_DrawIndex = -1;
    u64 m_SortKey = -1;

    /**
     * These are not stored here directly, rather they are accessed by the m_DrawIndex in the RenderBatch.
     *      m4 m_Transform;
     *      i32 m_EntityID;
     */

    void ComputeSortKey();

    struct DrawCommandComparator {
        bool operator() (const DrawCommand& a, const DrawCommand& b) const;
    };

    static bool Compare(const DrawCommand& a, const DrawCommand& b) {
        return a.m_SortKey < b.m_SortKey;
    }
};

} // ZPG

#endif //WORKSPACE_DRAW_COMMAND_H
