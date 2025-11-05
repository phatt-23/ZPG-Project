#type vertex
#version 440 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in vec3 a_Tangent;

layout(std430, binding = 0) buffer MatricesStorageBuffer {
    mat4 View;
    mat4 Proj;
    mat4 ViewProj;
} ssbo_Matrices;

out vec2 v_TexCoord;

void main()
{
    v_TexCoord = a_TexCoord;

    mat4 viewNoTrans = ssbo_Matrices.View;
    viewNoTrans[3] = vec4(0.0, -2.0, 0.0, 1.0);

    // Transform only by rotation part of View and then by Projection
    vec4 pos = ssbo_Matrices.Proj * viewNoTrans * vec4(a_Pos, 1.0);
    gl_Position = pos.xyww;
}
