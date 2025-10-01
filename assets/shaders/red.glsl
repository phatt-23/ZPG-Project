#type vertex
#version 330

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec4 a_Color;

uniform mat4 u_Model;
uniform mat4 u_ViewProj;

void main() {
    gl_Position = u_ViewProj * u_Model * vec4(a_Pos, 1.0);
}


#type fragment
#version 330

out vec4 o_Color;

void main() {
    o_Color = vec4(1.0, 0.0, 0.0, 1.0);
}
