#type fragment
#version 440 core

#include "ext/ssbo/CameraSSBO.glsl"
#include "ext/ssbo/EnvironmentLightSSBO.glsl"
#include "ext/ssbo/PointLightSSBO.glsl"
#include "ext/ssbo/SpotLightSSBO.glsl"

#define CALC_SHADOW_GLSL_IMPLEMENTATION
#include "ext/shadow/CalcShadow.glsl"

#define CONVERSIONS_GLSL_IMPLEMENTATION
#include "ext/conversions.glsl"

#include "ext/utils.glsl"

#define CALC_LIGHT_GLSL_IMPLEMENTATION
#include "ext/light/CalcLight.glsl"

in vec2 v_TexCoord;

const int SkyTypeNone = 0;
const int SkyTypeSkybox = 1 << 0;
const int SkyTypeSkydome = 1 << 1;

uniform int u_SkyType;
uniform sampler2D u_SkydomeMap;
uniform samplerCube u_SkyboxMap;

uniform sampler2D g_PositionMap;
uniform sampler2D g_NormalMap;
uniform sampler2D g_AlbedoMetallicMap;
uniform sampler2D g_EmissiveRoughnessMap;
uniform isampler2D g_EntityIDMap;

uniform sampler2DArray u_DirectionalLightShadowMapArray;
uniform sampler2DArray u_SpotLightShadowMapArray;
uniform samplerCubeArray u_PointLightShadowCubeMapArray;

layout(location = 0) out vec4 f_Color;  // viewing result
layout(location = 1) out int f_EntityID;   // entityID
layout(location = 2) out vec4 f_Brightness;  // brightness

void main()
{
    // extract material properties from maps
    vec3 P = texture(g_PositionMap, v_TexCoord).rgb;
    vec3 worldNormal = texture(g_NormalMap, v_TexCoord).rgb;
    int entityID = texture(g_EntityIDMap, v_TexCoord).r;

    PBRProps pbr;
    pbr.Albedo = vec4(texture(g_AlbedoMetallicMap, v_TexCoord).rgb, 1.0);
    pbr.Metallic = texture(g_AlbedoMetallicMap, v_TexCoord).a;
    pbr.Roughness = texture(g_EmissiveRoughnessMap, v_TexCoord).a;
    pbr.Emissive = vec4(texture(g_EmissiveRoughnessMap, v_TexCoord).rgb, 1.0);

    PhongProps phong = ConvertPBRToPhong(pbr);

    vec3 N = normalize(worldNormal);
    vec3 V = normalize(ssbo_Camera.CameraPosition - P);

    vec3 Lo = vec3(0.0);
    vec3 Le = ScaleByA(phong.DiffuseColor * phong.EmissiveColor);
    vec3 La = ScaleByA(phong.DiffuseColor * ssbo_EnvironmentLight.AmbientColor);

    // directional light
    {
        DirectionalLight light = ssbo_EnvironmentLight.DirLight;
        float shadow = CalcShadowDirectional(u_DirectionalLightShadowMapArray, P, N, light, ssbo_Camera.View);
        Lo += CalcLightDirectional(light, phong, V, N, shadow);
    }

    // point lights
    for (int i = 0; i < ssbo_PointLight.Count; i++)
    {
        PointLight light = ssbo_PointLight.LightArray[i];
        float shadow = 0.0;
        Lo += CalcLightPoint(light, phong, P, V, N, shadow);
    }

    // spotlights
    for (int i = 0; i < ssbo_SpotLight.Count; i++)
    {
        SpotLight light = ssbo_SpotLight.LightArray[i];
        float shadow = 0.0;
        Lo += CalcLightSpot(light, phong, P, V, N, shadow);
    }

    vec3 color = Lo + La + Le;

    f_Color = vec4(color, 1.0);
    f_EntityID = entityID;
    f_Brightness = vec4(CalcBrightnessSurpass(color), 1.0);
}

