#ifndef UTILS_GLSL
#define UTILS_GLSL

vec3 ScaleByA(vec4 v);

#endif

#ifdef UTILS_GLSL_IMPLEMENTATION

vec3 ScaleByA(vec4 v)
{
    return vec3(v) * v.a;
}

#endif
