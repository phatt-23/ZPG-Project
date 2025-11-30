#ifndef LIGHTING_PROPS_GLSL
#define LIGHTING_PROPS_GLSL

struct PBRProps
{
    vec4 Albedo;
    float Metallic;
    float Roughness;
    vec4 Emissive;
};

struct PhongProps
{
    vec4 DiffuseColor;
    float Shininess;
    vec4 SpecularColor;
    vec4 EmissiveColor;
};


#endif