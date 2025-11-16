#type fragment
#version 440 core

#include "ext/ssbo/CameraSSBO.glsl"
#include "ext/ssbo/MaterialSSBO.glsl"
#include "ext/ssbo/SpotLightSSBO.glsl"
#include "ext/ssbo/PointLightSSBO.glsl"
#include "ext/ssbo/EnvironmentLightSSBO.glsl"

uniform sampler2D u_AlbedoMap;
uniform sampler2D u_RoughnessMap;
uniform sampler2D u_MetalnessMap;
uniform sampler2D u_NormalMap;
uniform sampler2D u_EmissiveMap;

in vec3 v_WorldPos;
in vec3 v_WorldNormal;
in vec2 v_TexCoord;
in mat3 v_TBN;
in flat int v_EntityID;

layout(location = 0) out vec4 f_Color0;
layout(location = 1) out int f_Color1;

void main()
{
    vec3 texAlbedo = texture(u_AlbedoMap, v_TexCoord).rgb;
    vec3 texEmissive = texture(u_EmissiveMap, v_TexCoord).rgb;
    float texMetallic = texture(u_MetalnessMap, v_TexCoord).r;
    float texRoughness = texture(u_RoughnessMap, v_TexCoord).r;

    vec3 albedo = pow(texAlbedo, vec3(2.2)) * ssbo_Material.Albedo.rgb;
    vec3 emissive = texEmissive * ssbo_Material.Emissive.rgb * ssbo_Material.Emissive.a;
    float metallic = clamp(texMetallic * ssbo_Material.Metallic, 0.0, 1.0);
    float roughness = clamp(texRoughness * ssbo_Material.Roughness, 0.0, 1.0);

    vec3 diffuseColor = albedo * clamp(1.0 - metallic, 0.01, 1.0);
    float shininess = max(pow(1.0 - roughness, 4.0) * 512.0, 16.0);
    vec3 baseSpecColor = mix(vec3(0.04), albedo, metallic);
    float specIntensity = mix(0.5, 2.0, metallic) * mix(0.2, 1.0, pow(1.0 - roughness + 0.001, 2.0));
    vec3 specularColor = baseSpecColor * specIntensity;

    vec3 tangentNormal  = 2.0 * texture(u_NormalMap, v_TexCoord).rgb - 1.0;
    vec3 N = normalize(v_TBN * tangentNormal);
    vec3 V = normalize(ssbo_Camera.CameraPosition - v_WorldPos);

    vec3 Lo = vec3(0.0);
    vec3 Le = texture(u_EmissiveMap, v_TexCoord).rgb * ssbo_Material.Emissive.rgb * ssbo_Material.Emissive.a;

    // ambient light
    vec3 La = ssbo_EnvironmentLight.AmbientColor.rgb * ssbo_EnvironmentLight.AmbientColor.a * albedo;

    // directional light
    {
        DirectionalLight light = ssbo_EnvironmentLight.DirLight;
        vec3 L = normalize(-light.Direction);
        vec3 H = normalize(L + V);

        float NdotL = max(dot(N, L), 0.0);
        float NdotH = max(dot(N, H), 0.0);

        vec3 diffuse = NdotL * diffuseColor;
        vec3 specular = pow(NdotH, shininess) * specularColor;

        Lo += (diffuse + specular) * (light.Color.rgb * light.Color.a);
    }

    // point lights
    for (int i = 0; i < ssbo_PointLight.Count; i++)
    {
        PointLight light = ssbo_PointLight.LightArray[i];

        vec3 L = normalize(light.Position - v_WorldPos);
        vec3 H = normalize(L + V);
        float dist = length(light.Position - v_WorldPos);
        float atten = min(1.0 / (light.Attenuation.x * dist * dist + light.Attenuation.y * dist + light.Attenuation.z), 1.0);

        float NdotL = max(dot(N, L), 0.0);
        float NdotH = max(dot(N, H), 0.0);

        vec3 diffuse = NdotL * diffuseColor;
        vec3 specular = pow(NdotH, shininess) * specularColor;

        Lo += (diffuse + specular) * (light.Color.rgb * light.Color.a) * atten;
    }

    // spotlights
    for (int i = 0; i < ssbo_SpotLight.Count; i++)
    {
        SpotLight light = ssbo_SpotLight.LightArray[i];

        vec3 L = normalize(light.Position - v_WorldPos);
        vec3 H = normalize(V + L);
        float dist = length(light.Position - v_WorldPos);
        float atten = min(1.0 / (light.Attenuation.x * dist * dist + light.Attenuation.y * dist + light.Attenuation.z), 1.0);

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

        vec3 diffuse = NdotL * diffuseColor;
        vec3 specular = pow(NdotH, shininess) * specularColor;

        Lo += (diffuse + specular) * (light.Color.rgb * light.Color.a) * beamContrib * atten;
    }

    vec3 color = Lo + La + Le;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    f_Color0 = vec4(color, 1.0);
    f_Color1 = v_EntityID;
}
