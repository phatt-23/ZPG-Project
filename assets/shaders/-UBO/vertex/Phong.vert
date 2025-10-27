#type vertex
#version 430 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;

out vec3 v_WorldPos;
out vec3 v_WorldNormal;

layout (std140, binding = 0) uniform MatricesUBO {
    mat4 View;
    mat4 Proj;
    mat4 ViewProj;
} u_Matrices;

uniform mat4 u_Model;

void main() {
    vec4 worldPos = u_Model * vec4(a_Pos, 1.0);
    mat3 normalMat = transpose(inverse(mat3(u_Model)));

    v_WorldPos = vec3(worldPos);
    v_WorldNormal = normalMat * a_Normal;

    gl_Position = u_Matrices.ViewProj * worldPos;
}


