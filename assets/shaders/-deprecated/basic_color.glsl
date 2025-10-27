#type vertex
#version 330

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec4 a_Color;

uniform mat4 u_Model;
uniform mat4 u_ViewProj;

out vec4 v_Color;

void main() {
    gl_Position = u_ViewProj * u_Model * vec4(a_Pos, 1.0);
    
    v_Color = a_Color;
}

#type fragment
#version 330

in vec4 v_Color;
out vec4 f_Color;

void main() {
    f_Color = v_Color;
}
