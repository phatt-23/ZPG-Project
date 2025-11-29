#ifndef ATTENUATE_GLSL
#define ATTENUATE_GLSL

float Attenuate(float distance, float quadratic, float linear, float constant);
float Attenuate(float distance, vec3 attenuation);

#endif

#ifdef ATTENUATE_GLSL_IMPLEMENTATION

float Attenuate(float distance, float quadratic, float linear, float constant)
{
    return 1.0 / (quadratic * distance * distance + linear * distance + constant + 0.0001);
}

float Attenuate(float distance, vec3 attenuation)
{
    return Attenuate(distance, attenuation.x, attenuation.y, attenuation.z);
}

#endif

