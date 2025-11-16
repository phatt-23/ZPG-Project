#ifndef CAMERA_SSBO_GLSL
#define CAMERA_SSBO_GLSL

#include "ext/constants.glsl"

layout (std430, binding = BINDING_POINT_CAMERA_SSBO) buffer CameraShaderStorageBuffer
{
    mat4 View;
    mat4 Proj;
    mat4 ViewProj;
    vec3 CameraPosition;
    float FarPlane;
} ssbo_Camera;

#endif

