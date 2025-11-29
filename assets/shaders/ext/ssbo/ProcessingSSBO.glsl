#ifndef PROCESSING_SSBO_GLSL
#define PROCESSING_SSBO_GLSL

#include "ext/constants.glsl"

layout (std430, binding = BINDING_POINT_PROCESSING_SSBO) buffer ProcessingShaderStorageBuffer
{
    float Exposure;
    float Gamma;
    int BloomAmount;
} ssbo_Processing;

#endif

