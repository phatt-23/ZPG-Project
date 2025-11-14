#ifndef CALC_SHADOW_GLSL
#define CALC_SHADOW_GLSL

float CalcShadowDirectional(sampler2D sampler, vec4 worldPos, vec3 worldNormal);
float CalcShadowSpotLight(sampler2DArray sampler, vec4 worldPos, vec3 worldNormal, SpotLight light);
float CalcShadowPointLight(samplerCubeArray sampler, vec4 worldPos4, vec3 worldNormal, PointLight light);

#endif

#ifdef CALC_SHADOW_GLSL_IMPLEMENTATION

#define SHADOW_HELPERS_GLSL_IMPLEMENTATION
#include "ext/shadow/ShadowHelpers.glsl"

#define SHADOW_PCF_GLSL_IMPLEMENTATION
#include "ext/shadow/PCF.glsl"

float CalcShadowDirectional(sampler2D sampler, vec4 worldPos, vec3 worldNormal)
{
    DirectionalLight dirLight = ssbo_EnvironmentLight.DirLight;
    vec3 lightDir = dirLight.Direction;
    vec4 worldPosLightSpace = dirLight.ViewProj * worldPos;

    vec3 projCoord = worldPosLightSpace.xyz / worldPosLightSpace.w;
    projCoord = 0.5 * projCoord + 0.5;

    if (projCoord.z > 1.0 || projCoord.x > 1.0 || projCoord.y > 1.0 || projCoord.x < 0.0 || projCoord.y < 0.0 || projCoord.z < 0.0)
        return 0.0; // when oversampling, return no shadow

    float bias = ShadowBias(worldNormal, lightDir);
    int size = 4;

    float shadow = PCF(sampler, projCoord, bias, size);
    return shadow;
}

float CalcShadowSpotLight(sampler2DArray sampler, vec4 worldPos, vec3 worldNormal, SpotLight light)
{
    int shadowLayer = light.Index;
    vec3 lightDir = light.Direction;
    vec4 worldPosLightSpace = light.ViewProj * worldPos;

    vec3 projCoord = worldPosLightSpace.xyz / worldPosLightSpace.w;
    projCoord = 0.5 * projCoord + 0.5;

    if (projCoord.z > 1.0 || projCoord.x > 1.0 || projCoord.y > 1.0 || projCoord.x < 0.0 || projCoord.y < 0.0 || projCoord.z < 0.0)
        return 0.0; // when oversampling, return no shadow

    float bias = 0.00005; //ShadowBias(worldNormal, lightDir);
    int size = 4;

    float shadow = PCF(sampler, shadowLayer, projCoord, bias, size);

    return shadow;
}

float CalcShadowPointLight(samplerCubeArray sampler, vec4 worldPos4, vec3 worldNormal, PointLight light)
{
    vec3 Lvec = worldPos4.xyz / worldPos4.w - light.Position;
    float currentDepth = length(Lvec);

    vec3 Ldir = normalize(Lvec);

    float bias = ShadowBias(worldNormal, Ldir);

    // cheap test (no PCF)
    // float closestDepthN = texture(u_PointLightShadowCubeMapArray, vec4(Ldir, float(light.Index))).r;
    // float closestDepth = closestDepthN * light.Radius; // use light's FarPlane (not camera far)
    // float shadow = (currentDepth - bias > closestDepth) ? 1.0 : 0.0;

    float shadow = PCF(sampler, light.Index, Ldir, currentDepth, bias, light.Radius);

    return shadow;
}


#endif

