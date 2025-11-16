#ifndef SHADOW_HELPERS_GLSL
#define SHADOW_HELPERS_GLSL

float ShadowBias(vec3 worldNormal, vec3 lightDir);
bool IsWithin(float v, float min, float max);
bool WithinNDCBounds(vec3 projCoord);

#endif

#ifdef SHADOW_HELPERS_GLSL_IMPLEMENTATION

float ShadowBias(vec3 worldNormal, vec3 lightDir)
{
    float bias = max(0.005 * (1.0 - dot(worldNormal, lightDir)), 0.005);
    return bias;
}

bool IsWithin(float v, float min, float max)
{
    return v <= max && v >= min;
}

bool WithinNDCBounds(vec3 projCoord)
{
    float x = projCoord.x;
    float y = projCoord.y;
    float z = projCoord.z;
    return IsWithin(x, -1.0, 1.0) && IsWithin(y, -1.0, 1.0) && IsWithin(z, -1.0, 1.0);
}

#endif
