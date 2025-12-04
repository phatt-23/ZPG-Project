#type vertex
#version 440 core

#include "ext/ssbo/CameraSSBO.glsl"

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Tangent;

uniform mat4 u_Model;
out vec2 v_TexCoord;

void main()
{
    v_TexCoord = a_TexCoord;
    gl_Position = ssbo_Camera.ViewProj * u_Model * vec4(a_Pos, 1.0);
}
