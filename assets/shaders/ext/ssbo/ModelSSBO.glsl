#ifndef MODEL_SSBO_GLSL
#define MODEL_SSBO_GLSL

#include "ext/constants.glsl"

layout (std430, binding = BINDING_POINT_MODEL_SSBO) buffer ModelShaderStorageBuffer
{
    int ModelCount;
    mat4 Models[];
} ssbo_Model;

#endif