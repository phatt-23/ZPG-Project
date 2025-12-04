#ifndef DEBUG_SSBO_GLSL
#define DEBUG_SSBO_GLSL

#include "ext/constants.glsl"

const uint DEBUG_FLAG_SHOW_DIRECTIONAL_SHADOW_CASCADES = 1 << 0;
const uint DEBUG_FLAG_RENDER_DIRECTIONAL_CASCADE_SHADOW_MAP = 1 << 1;

layout (std430, binding = BINDING_POINT_DEBUG_SSBO) buffer DebugShaderStorageBuffer
{
    uint Flags;
    uint DirectionalCascadeIndex;
} ssbo_Debug;

#endif
