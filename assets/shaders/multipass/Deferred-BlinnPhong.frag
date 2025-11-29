#type fragment
#version 440 core

#include "ext/ssbo/CameraSSBO.glsl"
#include "ext/ssbo/EnvironmentLightSSBO.glsl"
#include "ext/ssbo/PointLightSSBO.glsl"
#include "ext/ssbo/SpotLightSSBO.glsl"
#include "ext/ssbo/ProcessingSSBO.glsl"

#define ATTENUATE_GLSL_IMPLEMENTATION
#include "ext/light/Attenuate.glsl"

#define CALC_SHADOW_GLSL_IMPLEMENTATION
#include "ext/shadow/CalcShadow.glsl"

#define CONVERSIONS_GLSL_IMPLEMENTATION
#include "ext/conversions.glsl"

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
    vec3 worldPos = texture(g_Color0, v_TexCoord).rgb;
    vec3 worldNormal = texture(g_Color1, v_TexCoord).rgb;

    PBRProps pbr;
    pbr.Albedo = texture(g_Color2, v_TexCoord).rgb; 
    pbr.Metallic = texture(g_Color2, v_TexCoord).a;
    pbr.Emissive = texture(g_Color3, v_TexCoord).rgb;
    pbr.Roughness = texture(g_Color3, v_TexCoord).a;

    PhongProps phong = ConvertPBRToPhong(pbr);
    phong.DiffuseColor = pow(phong.DiffuseColor, vec3(ssbo_Processing.Gamma));  // compensate because of gamma correction

    vec3 N = normalize(worldNormal);
    vec3 V = normalize(ssbo_Camera.CameraPosition - worldPos);

    vec3 Lo = vec3(0.0);
    vec3 Le = phong.DiffuseColor * phong.EmissiveColor;
    vec3 La = phong.DiffuseColor * ssbo_EnvironmentLight.AmbientColor.rgb * ssbo_EnvironmentLight.AmbientColor.a;

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

        float NdotL = dot(N, L);
        float NdotH = max(dot(N, H), 0.0);

        vec3 diffuse = max(NdotL, 0.0) * phong.DiffuseColor;
        vec3 specular = pow(NdotH, phong.Shininess) * phong.SpecularColor * int(NdotL >= 0.0);

        Lo += (diffuse + specular) * (light.Color.rgb * light.Color.a) * shadow;
    }

    // point lights
    for (int i = 0; i < ssbo_PointLight.Count; i++)
    {
        PointLight light = ssbo_PointLight.LightArray[i];

        vec3 L = normalize(light.Position - worldPos);
        vec3 H = normalize(V + L);
        float atten = Attenuate(length(light.Position - worldPos), light.Intensity, light.Attenuation);

        float NdotL = dot(N, L);
        float NdotH = max(dot(N, H), 0.0);

        vec3 diffuse = max(NdotL, 0.0) * phong.DiffuseColor;
        vec3 specular = pow(NdotH, phong.Shininess) * phong.SpecularColor * int(NdotL >= 0.0);

        Lo += (diffuse + specular) * (light.Color.rgb * light.Color.a) * atten;
    }

    // spotlights
    for (int i = 0; i < ssbo_SpotLight.Count; i++)
    {
        SpotLight light = ssbo_SpotLight.LightArray[i];

        vec3 L = normalize(light.Position - worldPos);
        vec3 H = normalize(V + L);
        float atten = Attenuate(length(light.Position - worldPos), light.Intensity, light.Attenuation);

        float lightBeamSize = light.BeamSize;
        float lightBeamBlend = max(light.BeamBlend, 0.01);

        float beamSizeCos = cos(radians(lightBeamSize));
        float beamBlendCos = cos(radians(lightBeamSize * (1.0 - lightBeamBlend)));

        if (beamBlendCos <= beamSizeCos) {
            continue;
        }

        float alpha = dot(normalize(light.Direction), -L);
        float beamContrib = clamp((alpha - beamSizeCos) / (beamBlendCos - beamSizeCos), 0.0, 1.0);

        float NdotL = dot(N, L);
        float NdotH = max(dot(N, H), 0.0);

        vec3 diffuse = max(NdotL, 0.0) * phong.DiffuseColor;
        vec3 specular = pow(NdotH, phong.Shininess) * phong.SpecularColor * int(NdotL >= 0.0);

        Lo += (diffuse + specular) * (light.Color.rgb * light.Color.a) * beamContrib * atten;
    }

    vec3 color = Lo + La + Le;

    int entityID = texture(g_Color4, v_TexCoord).r;
    float brightness = dot(color, vec3(0.7, 0.7, 0.7));

    // f_Color0 = vec4(gammaCorrectedColor, 1.0);
    f_Color0 = vec4(color, 1.0);
    f_Color1 = entityID;
    f_Color2 = vec4((brightness > 1.0) ? color : vec3(0.0), 1.0);
    // f_Color2 = vec4(color, 1.0);

}

