#type vertex
#version 330

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;

uniform mat4 u_Model;
uniform mat4 u_ViewProj;

out vec3 f_Normal;

void main() {
    gl_Position = u_ViewProj * u_Model * vec4(a_Pos, 1.0);
    
    f_Normal = a_Normal;
}


#type fragment
#version 330

in vec3 f_Normal;
out vec4 o_Color;

void main() {
    o_Color = vec4(f_Normal, 1.f);
}
