#ifndef ENVIRONMENT_LIGHT_SSBO_GLSL
#define ENVIRONMENT_LIGHT_SSBO_GLSL

#include "ext/constants.glsl"
#include "ext/light/DirectionalLight.glsl"

layout (std430, binding = ENVIRONMENT_LIGHT_SSBO_BINDING_POINT) buffer EnvironmentLightShaderStorageBuffer
{
    vec4 AmbientColor;
    DirectionalLight DirLight;
} ssbo_EnvironmentLight;

#endif