#ifndef CALC_SHADOW_GLSL
#define CALC_SHADOW_GLSL

#include "ext/light/PointLight.glsl"
#include "ext/light/SpotLight.glsl"
#include "ext/light/DirectionalLight.glsl"

float CalcShadowDirectional(
    sampler2DArray sampler, 
    vec3 worldPos, 
    vec3 worldNormal, 
    DirectionalLight light,
    mat4 cameraView
);

float CalcShadowSpotLight(sampler2DArray sampler, vec3 worldPos, vec3 worldNormal, SpotLight light);
float CalcShadowPointLight(samplerCubeArray sampler, vec3 worldPos, vec3 worldNormal, PointLight light);

#endif

#ifdef CALC_SHADOW_GLSL_IMPLEMENTATION

#define SHADOW_HELPERS_GLSL_IMPLEMENTATION
#include "ext/shadow/ShadowHelpers.glsl"

#define SHADOW_PCF_GLSL_IMPLEMENTATION
#include "ext/shadow/PCF.glsl"

float CalcShadowDirectional(
    sampler2DArray sampler, 
    vec3 worldPos, 
    vec3 worldNormal, 
    DirectionalLight light,
    mat4 cameraView
)
{
    // select cascade layer
    int sliceIndex = -1;
    vec4 worldPosViewSpace4 = cameraView * vec4(worldPos, 1.0);
    vec3 worldPosViewSpace = worldPosViewSpace4.xyz / worldPosViewSpace4.w;
    float depthValue = abs(worldPosViewSpace.z);
        
    for (int i = 0; i < light.CascadeCount; ++i)
    {
        if (depthValue < light.PlaneDistance[i])
        {
            sliceIndex = i;
            break;
        }
    }

    if (sliceIndex == -1)
    {
        sliceIndex = light.CascadeCount - 1;
    }
        
    vec3 lightDir = light.Direction;
    vec4 worldPosLightSpace = light.ViewProj[sliceIndex] * vec4(worldPos, 1.0);

    vec3 projCoord = worldPosLightSpace.xyz / worldPosLightSpace.w;
    projCoord = 0.5 * projCoord + 0.5;

    if (projCoord.z > 1.0 || projCoord.x > 1.0 || projCoord.y > 1.0 || projCoord.x < 0.0 || projCoord.y < 0.0 || projCoord.z < 0.0)
    {
        return 0.0; // when oversampling, return no shadow
    }

    float bias = max(0.05 * (1.0 - dot(worldNormal, lightDir)), 0.005) * 1.0f/(0.5f * light.PlaneDistance[sliceIndex]);

    // float bias = ShadowBias(worldNormal, lightDir);
    int size = 4;


    // float currentDepth = projCoord.z;
    // float texDepth = texture(sampler, vec3(projCoord.xy, sliceIndex)).r;
    // float shadow = (currentDepth - bias > texDepth) ? 1.0 : 0.0;
    float shadow = PCF(sampler, sliceIndex, projCoord, bias, size);

    return shadow;
}

float CalcShadowSpotLight(sampler2DArray sampler, vec3 worldPos, vec3 worldNormal, SpotLight light)
{
    int shadowLayer = light.Index;
    vec3 lightDir = light.Direction;
    vec4 worldPosLightSpace = light.ViewProj * vec4(worldPos, 1.0);

    vec3 projCoord = worldPosLightSpace.xyz / worldPosLightSpace.w;
    projCoord = 0.5 * projCoord + 0.5;

    if (projCoord.z > 1.0 || projCoord.x > 1.0 || projCoord.y > 1.0 || projCoord.x < 0.0 || projCoord.y < 0.0 || projCoord.z < 0.0)
        return 0.0; // when oversampling, return no shadow

    float bias = 0.00005; //ShadowBias(worldNormal, lightDir);
    int size = 4;

    float shadow = PCF(sampler, shadowLayer, projCoord, bias, size);

    return shadow;
}

float CalcShadowPointLight(samplerCubeArray sampler, vec3 worldPos, vec3 worldNormal, PointLight light)
{
    vec3 Lvec = worldPos - light.Position;
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

