#ifndef SPOTLIGHT_SSBO_GLSL
#define SPOTLIGHT_SSBO_GLSL

#include "ext/constants.glsl"
#include "ext/light/SpotLight.glsl"

layout (std430, binding = SPOTLIGHT_SSBO_BINDING_POINT) buffer SpotLightShaderStorageBuffer
{
    int Count;
    SpotLight LightArray[];
} ssbo_SpotLight;

#endif
