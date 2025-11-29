#ifndef POINTLIGHT_GLSL
#define POINTLIGHT_GLSL

struct PointLight
{
    vec4    Color;
    vec3    Position;
    int     Index;
    vec3    Attenuation;
    float   Intensity;
    float   _pad0[3];
    float   Radius;
    mat4    ViewProj[6];
};

#endif
