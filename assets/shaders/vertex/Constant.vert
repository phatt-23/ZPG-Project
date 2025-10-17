#type vertex
#version 400 core

layout(location = 0) in vec3 a_Pos;

uniform mat4 u_Model;
uniform mat4 u_ViewProj;

void main() {
    gl_Position = u_ViewProj * u_Model * vec4(a_Pos, 1.0);
}
