#ifndef ENTITY_SSBO_GLSL
#define ENTITY_SSBO_GLSL

#include "ext/constants.glsl"

layout (std430, binding = BINDING_POINT_ENTITY_SSBO) buffer EntityShaderStorageBuffer
{
    int EntityCount;
    float pad[3];
    int EntityIDs[];
} ssbo_Entity;


#endif