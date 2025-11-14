#ifndef SPOTLIGHT_GLSL
#define SPOTLIGHT_GLSL

struct SpotLight
{
    vec4    Color;
    vec3    Position;
    int     Index;
    vec3    Direction;
    float   BeamSize;
    vec3    Attenuation;
    float   BeamBlend;
    float   Radius;
    float   _pad0[3];
    mat4    ViewProj;
};

#endif