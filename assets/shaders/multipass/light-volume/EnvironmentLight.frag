#type fragment
#version 440 core

// includes have to be in this order, i don't know why
#include "ext/ssbo/EnvironmentLightSSBO.glsl"

#include "ext/ssbo/CameraSSBO.glsl"

#define CALC_SHADOW_GLSL_IMPLEMENTATION
#include "ext/shadow/CalcShadow.glsl"

layout(location = 0) out vec4 f_Color0;

in vec2 v_TexCoord;

uniform sampler2D g_PositionMap;
uniform sampler2D g_NormalMap;
uniform sampler2D g_AlbedoMetallicMap;
uniform sampler2D g_EmissiveRoughnessMap;

uniform sampler2D u_DirectionalLightShadowMap;

void main()
{
    vec3 worldPos = texture(g_PositionMap, v_TexCoord).rgb;
    vec3 N = normalize(texture(g_NormalMap, v_TexCoord).rgb * 2.0 - 1.0);
    vec3 albedo = texture(g_AlbedoMetallicMap, v_TexCoord).rgb;
    float metallic = texture(g_AlbedoMetallicMap, v_TexCoord).a;
    vec3 emissive = texture(g_EmissiveRoughnessMap, v_TexCoord).rgb;
    float roughness = texture(g_EmissiveRoughnessMap, v_TexCoord).a;

    vec3 diffuseColor = albedo * clamp(1.0 - metallic, 0.01, 1.0);
    float shininess = max(pow(1.0 - roughness, 4.0) * 512.0, 16.0);
    vec3 baseSpecColor = mix(vec3(0.04), albedo, metallic);
    float specIntensity = mix(0.5, 2.0, metallic) * mix(0.2, 1.0, pow(1.0 - roughness + 0.001, 2.0));
    vec3 specularColor = baseSpecColor * specIntensity;

    vec3 ambient = ssbo_EnvironmentLight.AmbientColor.rgb * ssbo_EnvironmentLight.AmbientColor.a;
    vec3 V = normalize(ssbo_Camera.CameraPosition - worldPos);

    // directional light
    DirectionalLight light = ssbo_EnvironmentLight.DirLight;

    float shadow = 1.0 - CalcShadowDirectional(u_DirectionalLightShadowMap, worldPos, N, light);

    vec3 L = normalize(-light.Direction);
    vec3 H = normalize(L + V);

    float NdotL = max(dot(N, L), 0.0);
    float NdotH = max(dot(N, H), 0.0);

    vec3 diffuse = NdotL * diffuseColor;
    vec3 specular = pow(NdotH, shininess) * specularColor;

    vec3 La = albedo * ambient;
    vec3 Lo = (diffuse + specular) * (light.Color.rgb * light.Color.a) * shadow;
    vec3 Le = albedo * emissive;

    vec3 color = Lo + Le + La;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));
    f_Color0 = vec4(color, 1.0);
}
