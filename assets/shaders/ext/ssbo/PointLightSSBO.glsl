#ifndef POINTLIGHT_SHADER_STORAGE_BUFFER
#define POINTLIGHT_SHADER_STORAGE_BUFFER

#include "ext/constants.glsl"
#include "ext/light/PointLight.glsl"

layout (std430, binding = POINTLIGHT_SSBO_BINDING_POINT) buffer PointLightShaderStorageBuffer
{
    int Count;
    PointLight LightArray[];
} ssbo_PointLight;

#endif
