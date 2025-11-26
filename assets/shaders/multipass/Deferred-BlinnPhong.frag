#type fragment
#version 440 core

#include "ext/ssbo/CameraSSBO.glsl"
#include "ext/ssbo/EnvironmentLightSSBO.glsl"
#include "ext/ssbo/PointLightSSBO.glsl"
#include "ext/ssbo/SpotLightSSBO.glsl"

#define CALC_SHADOW_GLSL_IMPLEMENTATION
#include "ext/shadow/CalcShadow.glsl"

in vec2 v_TexCoord;

const int SkyTypeNone = 0;
const int SkyTypeSkybox = 1 << 0;
const int SkyTypeSkydome = 1 << 1;

uniform int u_SkyType;
uniform sampler2D u_SkydomeMap;
uniform samplerCube u_SkyboxMap;

uniform sampler2D g_Color0; // pos
uniform sampler2D g_Color1; // normal
uniform sampler2D g_Color2; // albedo and metallic
uniform sampler2D g_Color3; // emissive and roughness
uniform isampler2D g_Color4; // entityID

uniform sampler2DArray u_DirectionalLightShadowMapArray;
uniform sampler2DArray u_SpotLightShadowMapArray;
uniform samplerCubeArray u_PointLightShadowCubeMapArray;

layout(location = 0) out vec4 f_Color0;
layout(location = 1) out int f_Color1;

void main()
{
    // extract material properties from maps
    vec3 worldPos = texture(g_Color0, v_TexCoord).rgb;
    vec3 worldNormal = texture(g_Color1, v_TexCoord).rgb * 2.0 - 1.0;

    vec3 albedo = texture(g_Color2, v_TexCoord).rgb; albedo = pow(albedo, vec3(2.2));
    float metallic = texture(g_Color2, v_TexCoord).a;
    vec3 emissive = texture(g_Color3, v_TexCoord).rgb;
    float roughness = texture(g_Color3, v_TexCoord).a;
    float ao = 1.0;

    vec3 diffuseColor = albedo * clamp(1.0 - metallic, 0.01, 1.0);
    float shininess = max(pow(1.0 - roughness, 4.0) * 512.0, 16.0);
    vec3 baseSpecColor = mix(vec3(0.04), albedo, metallic);
    float specIntensity = mix(0.5, 2.0, metallic) * mix(0.2, 1.0, pow(1.0 - roughness + 0.001, 2.0));
    vec3 specularColor = baseSpecColor * specIntensity;

    vec3 F0 = mix(vec3(0.04), albedo, metallic);

    vec3 N = normalize(worldNormal);
    vec3 V = normalize(ssbo_Camera.CameraPosition - worldPos);

    vec3 Lo = vec3(0.0);
    vec3 Le = emissive * albedo;
    vec3 La = albedo * ssbo_EnvironmentLight.AmbientColor.rgb * ssbo_EnvironmentLight.AmbientColor.a * ao;

    // directional light
    {
        DirectionalLight light = ssbo_EnvironmentLight.DirLight;

        float shadow = 1.0 - CalcShadowDirectional(
            u_DirectionalLightShadowMapArray, 
            worldPos, 
            worldNormal, 
            light, 
            ssbo_Camera.View);

        vec3 L = normalize(-light.Direction);
        vec3 H = normalize(L + V);

        float NdotL = max(dot(N, L), 0.0);
        float NdotH = max(dot(N, H), 0.0);

        vec3 diffuse = NdotL * diffuseColor;
        vec3 specular = pow(NdotH, shininess) * specularColor;

        Lo += (diffuse + specular) * (light.Color.rgb * light.Color.a) * shadow;
    }

    // point lights
    for (int i = 0; i < ssbo_PointLight.Count; i++)
    {
        PointLight light = ssbo_PointLight.LightArray[i];

        vec3 L = normalize(light.Position - worldPos);
        // vec3 R = reflect(-L, N);
        vec3 H = normalize(V + L);
        float dist = length(light.Position - worldPos);
        float atten = min(1.0 / (light.Attenuation.x * dist * dist + light.Attenuation.y * dist + light.Attenuation.z + 0.0001), 1.0);

        float NdotL = max(dot(N, L), 0.0);
        float NdotH = max(dot(N, H), 0.0);
        // float NdotH = max(dot(V, R), 0.0);

        vec3 diffuse = NdotL * diffuseColor;
        vec3 specular = pow(NdotH, shininess) * specularColor;

        Lo += (diffuse + specular) * (light.Color.rgb * light.Color.a) * atten;
    }

    // spotlights
    for (int i = 0; i < ssbo_SpotLight.Count; i++)
    {
        SpotLight light = ssbo_SpotLight.LightArray[i];

        vec3 L = normalize(light.Position - worldPos);
        // vec3 R = reflect(-L, N);
        vec3 H = normalize(V + L);
        float dist = length(light.Position - worldPos);
        float atten = min(1.0 / (light.Attenuation.x * dist * dist + light.Attenuation.y * dist + light.Attenuation.z + 0.00001), 1.0);

        float lightBeamSize = light.BeamSize;
        float lightBeamBlend = max(light.BeamBlend, 0.01);

        float beamSizeCos = cos(radians(lightBeamSize));
        float beamBlendCos = cos(radians(lightBeamSize * (1.0 - lightBeamBlend)));

        if (beamBlendCos <= beamSizeCos)
            continue;

        float alpha = dot(normalize(light.Direction), -L);
        float beamNumerator = alpha - beamSizeCos;
        float beamDenominator = beamBlendCos - beamSizeCos;
        float beamContrib = clamp(beamNumerator / beamDenominator, 0.0, 1.0);

        float NdotL = max(dot(N, L), 0.0);
        float NdotH = max(dot(N, H), 0.0);
        // float NdotH = max(dot(V, R), 0.0);

        vec3 diffuse = NdotL * diffuseColor;
        vec3 specular = pow(NdotH, shininess) * specularColor;

        Lo += (diffuse + specular) * (light.Color.rgb * light.Color.a) * beamContrib * atten;
    }

    vec3 color = Lo + La + Le;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));




    // select cascade layer (debug view)
    /*
    vec4 worldPosViewSpace4 = ssbo_Camera.View * vec4(worldPos, 1.0);
    vec3 worldPosViewSpace = worldPosViewSpace4.xyz / worldPosViewSpace4.w;
    float depthValue = abs(worldPosViewSpace.z);
        
    DirectionalLight dirlight = ssbo_EnvironmentLight.DirLight;

    int sliceIndex = dirlight.CascadeCount - 1;
    for (int i = 0; i < dirlight.CascadeCount; ++i)
    {
        if (depthValue < dirlight.PlaneDistance[i])
        {
            sliceIndex = i;
            break;
        }
    }

    if (sliceIndex == 0)
        color = vec3(1, 0, 0);
    else if (sliceIndex == 1)
        color = vec3(0, 1, 0);
    else if (sliceIndex == 2)
        color = vec3(0, 0, 1);
    else if (sliceIndex == 3)
        color = vec3(1, 0, 1);
    */


    f_Color0 = vec4(color, 1.0);

    int entityID = texture(g_Color4, v_TexCoord).r;
    f_Color1 = entityID;
}

