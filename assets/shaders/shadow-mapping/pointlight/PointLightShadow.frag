#type fragment
#version 440 core

#include "ext/ssbo/PointLightSSBO.glsl"

uniform int u_Index;

in vec4 g_WorldPos;

void main() {
    PointLight light = ssbo_PointLight.LightArray[u_Index];
    float depth = length(g_WorldPos.xyz - light.Position) / light.Radius;
    gl_FragDepth = depth;
}
