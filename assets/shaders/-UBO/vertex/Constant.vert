#type vertex
#version 420 core

layout(location = 0) in vec3 a_Pos;

layout (std140, binding = 0) uniform MatricesUBO {
    mat4 View;
    mat4 Proj;
    mat4 ViewProj;
} u_Matrices;

uniform mat4 u_Model;

void main() {
    gl_Position = u_Matrices.ViewProj * u_Model * vec4(a_Pos, 1.0);
}
