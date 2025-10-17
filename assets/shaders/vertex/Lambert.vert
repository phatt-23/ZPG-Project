#type vertex
#version 400 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;

out vec3 v_WorldPos;
out vec3 v_WorldNormal;

uniform mat4 u_Model;
uniform mat4 u_ViewProj;

void main() {
    vec4 worldPos = u_Model * vec4(a_Pos, 1.0);
    mat3 normalMat = transpose(inverse(mat3(u_Model)));

    v_WorldPos = vec3(worldPos);
    v_WorldNormal = normalMat * a_Normal;

    gl_Position = u_ViewProj * worldPos;
}


