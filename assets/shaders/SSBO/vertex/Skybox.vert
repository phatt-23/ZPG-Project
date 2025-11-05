#type vertex
#version 440 core

layout(location = 0) in vec3 a_Pos;

layout(std430, binding = 0) buffer MatricesStorageBuffer {
    mat4 View;
    mat4 Proj;
    mat4 ViewProj;
} ssbo_Matrices;

out vec3 v_TexCoords;

void main() {
    v_TexCoords = a_Pos;
    mat4 viewNoTrans = mat4(mat3(ssbo_Matrices.View));
    vec4 pos = ssbo_Matrices.Proj * viewNoTrans * vec4(a_Pos, 1.0);
    gl_Position = pos.xyww;
}
