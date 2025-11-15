#type vertex
#version 440 core

#include "ext/ssbo/EnvironmentLightSSBO.glsl"

layout(location = 0) in vec3 a_Pos; // only need the objects position

uniform mat4 u_Model;

out vec4 v_WorldPos;

void main()
{
    mat4 lightViewProj = ssbo_EnvironmentLight.DirLight.ViewProj;
    v_WorldPos = u_Model * vec4(a_Pos, 1.0);
    gl_Position = lightViewProj * v_WorldPos;
}
