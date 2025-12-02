#ifndef CONVERSIONS_GLSL
#define CONVERSIONS_GLSL

#include "ext/light/LightingProps.glsl"

PhongProps ConvertPBRToPhong(PBRProps pbr);

#endif

#ifdef CONVERSIONS_GLSL_IMPLEMENTATION

PhongProps ConvertPBRToPhong(PBRProps pbr)
{
    PhongProps phong;

    phong.DiffuseColor = pbr.Albedo * clamp(1.0 - pbr.Metallic, 0.2, 1.0);
    phong.Shininess = max(pow(1.0 - pbr.Roughness, 4.0) * 512.0, 16.0);

    vec4 baseSpecColor = vec4(mix(vec3(0.04), pbr.Albedo.rgb, pbr.Metallic), pbr.Albedo.a);
    float specIntensity = mix(0.5, 2.0, pbr.Metallic) * mix(0.2, 1.0, pow(1.0 - pbr.Roughness + 0.001, 2.0));
    phong.SpecularColor = baseSpecColor * specIntensity;
    phong.EmissiveColor = pbr.Emissive;

    return phong;
}

#endif
