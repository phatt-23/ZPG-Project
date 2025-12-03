#type fragment
#version 440 core

#include "ext/ssbo/EnvironmentLightSSBO.glsl"
#include "ext/ssbo/CameraSSBO.glsl"

#define CALC_SHADOW_GLSL_IMPLEMENTATION
#include "ext/shadow/CalcShadow.glsl"

#define CALC_LIGHT_GLSL_IMPLEMENTATION
#include "ext/light/CalcLight.glsl"

#define CONVERSIONS_GLSL_IMPLEMENTATION
#include "ext/conversions.glsl"

layout(location = 0) out vec4 f_Color;
layout(location = 2) out vec4 f_Brightness;  // brightness

in vec2 v_TexCoord;

uniform sampler2D g_PositionMap;
uniform sampler2D g_NormalMap;
uniform sampler2D g_AlbedoMetallicMap;
uniform sampler2D g_EmissiveRoughnessMap;

uniform sampler2DArray u_DirectionalLightShadowMapArray;

void main()
{
    // extract material properties from maps
    vec3 P = texture(g_PositionMap, v_TexCoord).rgb;
    vec3 worldNormal = texture(g_NormalMap, v_TexCoord).rgb;

    PBRProps pbr;
    pbr.Albedo = vec4(texture(g_AlbedoMetallicMap, v_TexCoord).rgb, 1.0);
    pbr.Metallic = texture(g_AlbedoMetallicMap, v_TexCoord).a;
    pbr.Roughness = texture(g_EmissiveRoughnessMap, v_TexCoord).a;
    pbr.Emissive = vec4(texture(g_EmissiveRoughnessMap, v_TexCoord).rgb, 1.0);

    PhongProps phong = ConvertPBRToPhong(pbr);

    vec3 N = normalize(worldNormal);
    vec3 V = normalize(ssbo_Camera.CameraPosition - P);

    // directional light
    DirectionalLight light = ssbo_EnvironmentLight.DirLight;

    float shadow = CalcShadowDirectional(u_DirectionalLightShadowMapArray, P, N, light, ssbo_Camera.View);

    vec3 Le = ScaleByA(phong.DiffuseColor * phong.EmissiveColor);
    vec3 La = ScaleByA(phong.DiffuseColor * ssbo_EnvironmentLight.AmbientColor);
    vec3 Lo = CalcLightDirectional(light, phong, V, N, shadow);

    vec3 color = Lo + Le + La;

    f_Color = vec4(color, 1.0);
    f_Brightness = vec4(CalcBrightnessSurpass(color), 1.0);
}
