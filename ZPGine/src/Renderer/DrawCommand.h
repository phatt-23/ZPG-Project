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

    // m4 m_Transform;
    u32 m_TransformIndex;


    struct DrawCommandComparator {
        bool operator() (const DrawCommand& a, const DrawCommand& b) const;
    };
};

} // ZPG

#endif //WORKSPACE_DRAW_COMMAND_H
