#type fragment
#version 440 core

#include "ext/ssbo/CameraSSBO.glsl"
#include "ext/ssbo/EnvironmentLightSSBO.glsl"
#include "ext/ssbo/PointLightSSBO.glsl"
#include "ext/ssbo/SpotLightSSBO.glsl"

#define PBR_GLSL_IMPLEMENTATION
#include "ext/pbr/PBR.glsl"

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

uniform sampler2D u_DirectionalLightShadowMap;
uniform sampler2DArray u_SpotLightShadowMapArray;
uniform samplerCubeArray u_PointLightShadowCubeMapArray;

layout(location = 0) out vec4 f_Color0;
layout(location = 1) out int f_Color1;

void main()
{
    // extract material properties from maps
    vec3 worldPos = texture(g_Color0, v_TexCoord).rgb;
    vec4 worldPos4 = texture(g_Color0, v_TexCoord).rgba;
    vec3 worldNormal = texture(g_Color1, v_TexCoord).rgb * 2.0 - 1.0;

    vec3 albedo = texture(g_Color2, v_TexCoord).rgb;
    albedo = pow(albedo, vec3(2.2));
    float metallic = texture(g_Color2, v_TexCoord).a;

    vec3 emissive = texture(g_Color3, v_TexCoord).rgb;
    float roughness = texture(g_Color3, v_TexCoord).a;

    float ao = 1.0;

    vec3 F0 = mix(vec3(0.04), albedo, metallic);

    vec3 N = normalize(worldNormal);
    vec3 V = normalize(ssbo_Camera.CameraPosition - worldPos);

    vec3 Lo = vec3(0.0);
    vec3 La = albedo * ssbo_EnvironmentLight.AmbientColor.rgb * ssbo_EnvironmentLight.AmbientColor.a * ao;

    // Reflectance Equation

    // Directional Light
    {
        vec4 directionalLightColor = ssbo_EnvironmentLight.DirLight.Color;
        vec3 lightColor = directionalLightColor.rgb * directionalLightColor.a;
        vec3 lightDir = ssbo_EnvironmentLight.DirLight.Direction;

        float shadow = 1.0 - CalcShadowDirectional(u_DirectionalLightShadowMap, worldPos4, N);

        // calculate per-light radiance
        vec3 L = normalize(-lightDir);
        vec3 H = normalize(V + L);
        vec3 radiance     = lightColor;

        // cook-torrance brdf
        float NDF = DistributionGGX(N, H, roughness);
        float G   = GeometrySmith(N, V, L, roughness);
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular     = numerator / denominator;

        // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * albedo / PI + specular) * radiance * NdotL * shadow;
    }

    // Point Lights

    for (int i = 0; i < ssbo_PointLight.Count; i++)
    {
        PointLight light = ssbo_PointLight.LightArray[i];

        float shadow = 1.0 - CalcShadowPointLight(u_PointLightShadowCubeMapArray, worldPos4, N, light);

        vec3 lightColor = light.Color.rgb;
        vec3 lightPos = light.Position.xyz;
        vec3 lightAtten = light.Attenuation;

        // calculate per-light radiance
        vec3 L = normalize(lightPos - worldPos);
        vec3 H = normalize(V + L);
        float distance    = length(lightPos - worldPos);
        float attenuation = min(light.Color.a / (lightAtten.x * distance * distance + lightAtten.y * distance + lightAtten.z), 1.0);
        vec3 radiance     = lightColor * attenuation;

        // cook-torrance brdf
        float NDF = DistributionGGX(N, H, roughness);
        float G   = GeometrySmith(N, V, L, roughness);
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular     = numerator / denominator;

        // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * albedo / PI + specular) * radiance * NdotL * shadow;
    }

    // Spot Lights

    for (int i = 0; i < ssbo_SpotLight.Count; i++)
    {
        SpotLight light = ssbo_SpotLight.LightArray[i];

        float shadow = 1.0 - CalcShadowSpotLight(u_SpotLightShadowMapArray, worldPos4, N, light);

        vec3 lightColor = light.Color.rgb;
        vec3 lightPos = light.Position.xyz;
        vec3 lightAtten = light.Attenuation;
        vec3 lightDirection = light.Direction;
        float lightBeamSize = light.BeamSize;
        float lightBeamBlend = light.BeamBlend;

        // calculate per-light radiance
        vec3 L = normalize(lightPos - worldPos);
        vec3 H = normalize(V + L);

        float beamSizeCos = cos(radians(lightBeamSize));
        float beamBlendCos = cos(radians(lightBeamSize * (1.0 - lightBeamBlend)));

        if (beamBlendCos <= beamSizeCos)
            continue;

        float alpha = dot(normalize(lightDirection), -L);
        float beamNumerator = alpha - beamSizeCos;
        float beamDenominator = beamBlendCos - beamSizeCos;
        float beamContrib = clamp(beamNumerator / beamDenominator, 0.0, 1.0);

        float distance    = length(lightPos - worldPos);
        float attenuation = min(light.Color.a / (lightAtten.x * distance * distance + lightAtten.y * distance + lightAtten.z), 1.0);
        vec3 radiance     = lightColor * attenuation * beamContrib;

        // cook-torrance brdf
        float NDF = DistributionGGX(N, H, roughness);
        float G   = GeometrySmith(N, V, L, roughness);
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular     = numerator / denominator;

        // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * albedo / PI + specular) * radiance * NdotL * shadow;
    }

    vec3 color = Lo + La + emissive;
    vec3 finalColor = color;

    if (u_SkyType == SkyTypeSkybox)
    {
        vec3 R = reflect(-V, N);
        vec3 envColor = texture(u_SkyboxMap, R).rgb; // sample environment reflection

        vec3 F = fresnelSchlick(max(dot(N, V), 0.0), F0); // Fresnel term — stronger near grazing angles

        float reflectionStrength = (1.0 - roughness);
        vec3 reflection = envColor * F * reflectionStrength; // Attenuate reflection by smoothness (1 - roughness)

        finalColor += reflection * 0.5; // Combine lighting with reflection
    }
    else if (u_SkyType == SkyTypeSkydome)
    {
        vec3 R = reflect(-V, N);

        vec3 dir = normalize(R);
        float theta = acos(clamp(dir.y, 0.0, 1.0)); // only upper hemisphere
        float phi = atan(dir.z, dir.x);
        float r = theta / (0.5 * PI);
        float u = 0.5 + 0.5 * r * cos(phi);
        float v = 0.5 + 0.5 * r * sin(phi);
        vec2 uv = vec2(u, v);

        vec3 envColor = texture(u_SkydomeMap, uv).rgb; // sample environment reflection

        vec3 F = fresnelSchlick(max(dot(N, V), 0.0), F0); // Fresnel term — stronger near grazing angles

        float reflectionStrength = (1.0 - roughness);
        vec3 reflection = envColor * F * reflectionStrength; // Attenuate reflection by smoothness (1 - roughness)

        finalColor = color + reflection * 0.5; // Combine lighting with reflection
    }

    // Tone mapping + gamma correction
    finalColor = finalColor / (finalColor + vec3(1.0));
    finalColor = pow(finalColor, vec3(1.0 / 2.2));

    f_Color0 = vec4(finalColor, 1.0);
    // f_Color0 = vec4(vec3(CalcPointLightShadowDebug(worldPos4, N, ssbo_PointLight.LightArray[0])), 1.0);
    f_Color1 = texture(g_Color4, v_TexCoord).r;
}


