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

uniform sampler2D g_Color0; // pos
uniform sampler2D g_Color1; // normal
uniform sampler2D g_Color2; // albedo and metallic
uniform sampler2D g_Color3; // emissive and roughness
uniform isampler2D g_Color4; // entityID

uniform sampler2DArray u_DirectionalLightShadowMapArray;
uniform sampler2DArray u_SpotLightShadowMapArray;
uniform samplerCubeArray u_PointLightShadowCubeMapArray;

layout(location = 0) out vec4 f_Color0;  // viewing result
layout(location = 1) out int f_Color1;   // entityID
layout(location = 2) out vec4 f_Color2;  // brightness

void main()
{
    // extract material properties from maps
    vec3 P = texture(g_Color0, v_TexCoord).rgb;
    vec3 worldNormal = texture(g_Color1, v_TexCoord).rgb;

    PBRProps pbr;
    pbr.Albedo = vec4(texture(g_Color2, v_TexCoord).rgb, 1.0);
    pbr.Metallic = texture(g_Color2, v_TexCoord).a;
    pbr.Roughness = texture(g_Color3, v_TexCoord).a;
    pbr.Emissive = vec4(texture(g_Color3, v_TexCoord).rgb, 1.0);

    PhongProps phong = ConvertPBRToPhong(pbr);
    // phong.DiffuseColor = pow(phong.DiffuseColor, vec4(vec3(ssbo_Processing.Gamma), 1.0));  // compensate because of gamma correction

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

    int entityID = texture(g_Color4, v_TexCoord).r;
    float brightness = dot(color, vec3(0.7, 0.7, 0.7));

    f_Color0 = vec4(color, 1.0);
    f_Color1 = entityID;
    f_Color2 = vec4((brightness > 1.0) ? color : vec3(0.0), 1.0);
}

