#ifndef MATERIAL_SSBO_GLSL
#define MATERIAL_SSBO_GLSL

#include "ext/constants.glsl"

layout (std430, binding = BINDING_POINT_MATERIAL_SSBO) buffer MaterialStorageBuffer
{
    vec4 Albedo;
    vec4 Emissive;
    float Roughness;
    float Metallic;
    float _pad[2];
    vec2 TilingFactor;
} ssbo_Material;

uniform sampler2D u_AlbedoMap;
uniform sampler2D u_RoughnessMap;
uniform sampler2D u_MetalnessMap;
uniform sampler2D u_NormalMap;
uniform sampler2D u_EmissiveMap;

#endif
