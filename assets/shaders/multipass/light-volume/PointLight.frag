#type fragment
#version 440 core

#include "ext/ssbo/CameraSSBO.glsl"
#include "ext/ssbo/PointLightSSBO.glsl"

#define CALC_SHADOW_GLSL_IMPLEMENTATION
#include "ext/shadow/CalcShadow.glsl"

#define CALC_LIGHT_GLSL_IMPLEMENTATION
#include "ext/light/CalcLight.glsl"

layout(location = 0) out vec4 f_Color;

in vec2 v_TexCoord;

uniform int u_Index;
uniform vec2 u_ScreenSizeInv;

uniform sampler2D g_PositionMap;
uniform sampler2D g_NormalMap;
uniform sampler2D g_AlbedoMetallicMap;
uniform sampler2D g_EmissiveRoughnessMap;

void main()
{
    f_Color = vec4(1,0,0,1);

    vec2 texCoord = gl_FragCoord.xy * u_ScreenSizeInv;
    // vec2 texCoord = v_TexCoord;

    // extract material properties from maps
    vec3 P = texture(g_PositionMap, texCoord).rgb;
    vec3 worldNormal = texture(g_NormalMap, texCoord).rgb;

    PBRProps pbr;
    pbr.Albedo = vec4(texture(g_AlbedoMetallicMap, texCoord).rgb, 1.0);
    pbr.Metallic = texture(g_AlbedoMetallicMap, texCoord).a;
    pbr.Roughness = texture(g_EmissiveRoughnessMap, texCoord).a;
    pbr.Emissive = vec4(texture(g_EmissiveRoughnessMap, texCoord).rgb, 1.0);

    PhongProps phong = ConvertPBRToPhong(pbr);

    vec3 N = normalize(worldNormal);
    vec3 V = normalize(ssbo_Camera.CameraPosition - P);

    PointLight light = ssbo_PointLight.LightArray[u_Index];

    // float shadow = 0.0;
    // vec3 Lo = CalcLightPoint(light, phong, P, V, N, shadow);

    // f_Color = vec4(N, 1.0);
    f_Color = vec4(1,0,0, 1.0);

    // f_Color = vec4(Lo, 1.0);
}
