#type vertex
#version 440 core

#include "ext/ssbo/SpotLightSSBO.glsl"

layout(location = 0) in vec3 a_Pos; // only need the objects position

uniform mat4 u_Model;
uniform int u_Index;

void main()
{
    SpotLight light = ssbo_SpotLight.LightArray[u_Index];

    mat4 lightViewProj = light.ViewProj;
    vec4 WorldPos = u_Model * vec4(a_Pos, 1.0);

    gl_Position = lightViewProj * WorldPos;
}
