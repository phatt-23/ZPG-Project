#type fragment
#version 440 core

#include "ext/ssbo/CameraSSBO.glsl"
#include "ext/ssbo/PointLightSSBO.glsl"

#define CALC_SHADOW_GLSL_IMPLEMENTATION
#include "ext/shadow/CalcShadow.glsl"

layout(location = 0) out vec4 f_Color0;

uniform int u_Index;
uniform vec2 u_ScreenSizeInv;

uniform sampler2D g_PositionMap;
uniform sampler2D g_NormalMap;
uniform sampler2D g_AlbedoMetallicMap;
uniform sampler2D g_EmissiveRoughnessMap;

void main()
{
    vec2 texCoord = gl_FragCoord.xy * u_ScreenSizeInv;

    vec3 worldPos = texture(g_PositionMap, texCoord).rgb;
    vec3 N = normalize(texture(g_NormalMap, texCoord).rgb * 2.0 - 1.0);
    vec3 albedo = texture(g_AlbedoMetallicMap, texCoord).rgb;
    float metallic = texture(g_AlbedoMetallicMap, texCoord).a;
    vec3 emissive = texture(g_EmissiveRoughnessMap, texCoord).rgb;
    float roughness = texture(g_EmissiveRoughnessMap, texCoord).a;

    vec3 diffuseColor = albedo * clamp(1.0 - metallic, 0.01, 1.0);
    float shininess = max(pow(1.0 - roughness, 4.0) * 512.0, 16.0);
    vec3 baseSpecColor = mix(vec3(0.04), albedo, metallic);
    float specIntensity = mix(0.5, 2.0, metallic) * mix(0.2, 1.0, pow(1.0 - roughness + 0.001, 2.0));
    vec3 specularColor = baseSpecColor * specIntensity;

    PointLight light = ssbo_PointLight.LightArray[u_Index];

    vec3 L = normalize(light.Position - worldPos);
    vec3 V = normalize(ssbo_Camera.CameraPosition - worldPos);
    vec3 H = normalize(V + L);

    float dist = length(light.Position - worldPos);
    float atten = min(1.0 / (light.Attenuation.x * dist * dist + light.Attenuation.y * dist + light.Attenuation.z), 1.0);

    float NdotL = max(dot(N, L), 0.0);
    float NdotH = max(dot(N, H), 0.0);

    vec3 diffuse = NdotL * diffuseColor;
    vec3 specular = pow(NdotH, shininess) * specularColor;

    vec3 Lo = (diffuse + specular) * (light.Color.rgb * light.Color.a) * atten;

    // Lo = Lo / (Lo + vec3(1.0));
    // Lo = pow(Lo, vec3(1.0 / 2.2));
    f_Color0 = vec4(Lo, 1.0);
}
