#type vertex
#version 440 core

layout(location = 0) in vec3 a_Pos; // only need the object's position

uniform mat4 u_Model;
out vec4 v_WorldPos;

void main()
{
    v_WorldPos = u_Model * vec4(a_Pos, 1.0);
    gl_Position = v_WorldPos;
}
