#ifndef CONVERSIONS_GLSL
#define CONVERSIONS_GLSL

struct PBRProps
{
    vec3 Albedo;
    float Metallic;
    float Roughness;
    vec3 Emissive;
};

struct PhongProps
{
    vec3 DiffuseColor;
    float Shininess;
    vec3 SpecularColor;
    vec3 EmissiveColor;
};

PhongProps ConvertPBRToPhong(PBRProps pbr);

#endif

#ifdef CONVERSIONS_GLSL_IMPLEMENTATION

PhongProps ConvertPBRToPhong(PBRProps pbr)
{
    PhongProps phong;

    phong.DiffuseColor = pbr.Albedo * clamp(1.0 - pbr.Metallic, 0.01, 1.0);
    phong.Shininess = max(pow(1.0 - pbr.Roughness, 4.0) * 512.0, 16.0);

    vec3 baseSpecColor = mix(vec3(0.04), pbr.Albedo, pbr.Metallic);
    float specIntensity = mix(0.5, 2.0, pbr.Metallic) * mix(0.2, 1.0, pow(1.0 - pbr.Roughness + 0.001, 2.0));
    phong.SpecularColor = baseSpecColor * specIntensity;
    phong.EmissiveColor = pbr.Emissive;

    return phong;
}

#endif
