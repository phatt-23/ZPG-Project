#ifndef CALC_LIGHT_GLSL
#define CALC_LIGHT_GLSL

#include "ext/light/DirectionalLight.glsl"
#include "ext/light/PointLight.glsl"
#include "ext/light/SpotLight.glsl"
#include "ext/light/LightingProps.glsl"

vec3 CalcLightDirectional( DirectionalLight light,
                           PhongProps phong,
                           vec3 V,
                           vec3 N,
                           float shadow );

vec3 CalcLightPoint( PointLight light,
                     PhongProps phong,
                     vec3 P,
                     vec3 V,
                     vec3 N,
                     float shadow );

vec3 CalcLightSpot( SpotLight light,
                    PhongProps phong,
                    vec3 P,
                    vec3 V,
                    vec3 N,
                    float shadow );

float CalcBrightness( vec3 color );
float CalcBrightness( vec3 color, vec3 contribution );

vec3 CalcBrightnessSurpass( vec3 color );
vec3 CalcBrightnessSurpass( vec3 color, float threshold );
vec3 CalcBrightnessSurpass( vec3 color, float threshold, vec3 contribution );

#endif

#ifdef CALC_LIGHT_GLSL_IMPLEMENTATION

#define ATTENUATE_GLSL_IMPLEMENTATION
#include "ext/light/Attenuate.glsl"

#define UTILS_GLSL_IMPLEMENTATION
#include "ext/utils.glsl"

vec3 CalcLightDirectional( DirectionalLight light, PhongProps phong, vec3 V, vec3 N, float shadow )
{
    vec3 L = normalize(-light.Direction);
    vec3 H = normalize(L + V);

    float NdotL = dot(N, L);
    float NdotH = max(dot(N, H), 0.0);

    vec3 diffuse = max(NdotL, 0.0) * ScaleByA(phong.DiffuseColor);
    vec3 specular = pow(NdotH, phong.Shininess) * ScaleByA(phong.SpecularColor) * int(NdotL >= 0.0);

    vec3 Lo = (diffuse + specular) * ScaleByA(light.Color) * (1.0 - shadow);
    return Lo;
}

vec3 CalcLightPoint( PointLight light, PhongProps phong, vec3 P, vec3 V, vec3 N, float shadow )
{
    vec3 L = normalize(light.Position - P);
    vec3 H = normalize(V + L);
    float atten = Attenuate(length(light.Position - P), light.Intensity, light.Attenuation);

    float NdotL = dot(N, L);
    float NdotH = max(dot(N, H), 0.0);

    vec3 diffuse = max(NdotL, 0.0) * ScaleByA(phong.DiffuseColor);
    vec3 specular = pow(NdotH, phong.Shininess) * ScaleByA(phong.SpecularColor) * int(NdotL >= 0.0);

    vec3 Lo = (diffuse + specular) * ScaleByA(light.Color) * atten * (1.0 - shadow);
    return Lo;
}

vec3 CalcLightSpot( SpotLight light, PhongProps phong, vec3 P, vec3 V, vec3 N, float shadow )
{
    vec3 L = normalize(light.Position - P);
    vec3 H = normalize(V + L);
    float atten = Attenuate(length(light.Position - P), light.Intensity, light.Attenuation);

    float lightBeamSize = light.BeamSize;
    float lightBeamBlend = max(light.BeamBlend, 0.01);

    float beamSizeCos = cos(radians(lightBeamSize));  // outer
    float beamBlendCos = cos(radians(lightBeamSize * (1.0 - lightBeamBlend))); // inner

    if (beamBlendCos <= beamSizeCos) {
        return vec3(0.0);
    }

    float alpha = dot(normalize(light.Direction), -L);
    float beamContrib = clamp((alpha - beamSizeCos) / (beamBlendCos - beamSizeCos), 0.0, 1.0);

    float NdotL = dot(N, L);
    float NdotH = max(dot(N, H), 0.0);

    vec3 diffuse = max(NdotL, 0.0) * ScaleByA(phong.DiffuseColor);
    vec3 specular = pow(NdotH, phong.Shininess) * ScaleByA(phong.SpecularColor) * int(NdotL >= 0.0);

    vec3 Lo = (diffuse + specular) * ScaleByA(light.Color) * beamContrib * atten;
    return Lo;
}

const vec3 CALC_LIGHT_DEFAULT_CONTRIBUTON = vec3(0.7, 0.7, 0.7);
const float CALC_LIGHT_DEFAULT_THRESHOLD = 1.0;

float CalcBrightness( vec3 color )
{
    return CalcBrightness( color, CALC_LIGHT_DEFAULT_CONTRIBUTON );
}
float CalcBrightness( vec3 color, vec3 contribution )
{
    return dot(color, contribution);
}

vec3 CalcBrightnessSurpass( vec3 color )
{
    return CalcBrightnessSurpass(
        color, 
        CALC_LIGHT_DEFAULT_THRESHOLD, 
        CALC_LIGHT_DEFAULT_CONTRIBUTON
    );
}

vec3 CalcBrightnessSurpass( vec3 color, float threshold )
{
    return CalcBrightnessSurpass(color, threshold, CALC_LIGHT_DEFAULT_CONTRIBUTON);
}

vec3 CalcBrightnessSurpass( vec3 color, float threshold, vec3 contribution )
{
    float brightness = CalcBrightness(color, contribution);
    return (brightness > threshold) ? color : vec3(0.0);
}

#endif
