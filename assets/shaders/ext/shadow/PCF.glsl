#ifndef SHADOW_PCF_GLSL
#define SHADOW_PCF_GLSL

float PCF(sampler2D sampler, vec3 projCoord, float bias, int size);
float PCF(sampler2DArray sampler, int slice, vec3 projCoord, float bias, int size);
float PCF(samplerCubeArray sampler, int slice, vec3 Ldir, float currentDepth, float bias, float lightFarPlane);

#endif

#ifdef SHADOW_PCF_GLSL_IMPLEMENTATION

float PCF(sampler2D sampler, vec3 projCoord, float bias, int size)
{
    float currentDepth = projCoord.z;
    vec2 texelSize = 1.0 / textureSize(sampler, 0);
    float shadow = 0.0;
    int i = 0;

    for(int x = -size; x <= size; ++x)
    {
        for(int y = -size; y <= size; ++y)
        {
            float pcfDepth = texture(sampler, projCoord.xy + vec2(x, y) * texelSize).r;
            shadow += (currentDepth - bias > pcfDepth) ? 1.0 : 0.0;
            i++;
        }
    }
    return shadow/i;
}

float PCF(sampler2DArray sampler, int slice, vec3 projCoord, float bias, int size)
{
    float currentDepth = projCoord.z;
    vec2 texelSize = 1.0 / textureSize(sampler, 0).xy;
    float shadow = 0.0;
    int i = 0;

    for(int x = -size; x <= size; ++x)
    {
        for(int y = -size; y <= size; ++y)
        {
            float pcfDepth = texture(sampler, vec3(projCoord.xy + vec2(x,y) * texelSize, float(slice))).r;
            shadow += (currentDepth - bias > pcfDepth) ? 1.0 : 0.0;
            i++;
        }
    }
    return shadow/i;
}

float PCF(samplerCubeArray sampler, int slice, vec3 Ldir, float currentDepth, float bias, float lightFarPlane) {
    const vec3 kernel[20] = vec3[](
        vec3(1, 1, 1), vec3( 1,-1, 1), vec3(-1, 1, 1), vec3(-1,-1, 1),
        vec3(1, 1,-1), vec3( 1,-1,-1), vec3(-1, 1,-1), vec3(-1,-1,-1),
        vec3(1, 1, 0), vec3( 1,-1, 0), vec3(-1, 1, 0), vec3(-1,-1, 0),
        vec3(1, 0, 1), vec3(-1, 0, 1), vec3( 1, 0,-1), vec3(-1, 0,-1),
        vec3(0, 1, 1), vec3( 0,-1, 1), vec3( 0, 1,-1), vec3( 0,-1,-1)
    );

    float shadow = 0.0;
    const float numSamples = 20.0;

    float diskRadius = 0.01;

    for (int i = 0; i < 20; ++i)
    {
        vec3 sampleDir = normalize(Ldir + kernel[i] * diskRadius);

        float depthSampleN = texture(sampler, vec4(sampleDir, float(slice))).r;

        float depthSample = depthSampleN * lightFarPlane;

        shadow += (currentDepth - bias > depthSample) ? 1.0 : 0.0;
    }

    return shadow / numSamples;
}

#endif
