#type vertex
#version 330

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;

uniform mat4 u_Model;
uniform mat4 u_ViewProj;

out vec3 v_Normal;

void main() {
    gl_Position = u_ViewProj * u_Model * vec4(a_Pos, 1.0);
    
    v_Normal = a_Normal;
}