#ifndef SPOTLIGHT_SSBO_GLSL
#define SPOTLIGHT_SSBO_GLSL

#include "ext/constants.glsl"
#include "ext/light/SpotLight.glsl"

layout (std430, binding = BINDING_POINT_SPOTLIGHT_SSBO) buffer SpotLightShaderStorageBuffer
{
    int Count;
    SpotLight LightArray[];
} ssbo_SpotLight;

#endif
