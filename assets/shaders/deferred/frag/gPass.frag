#type fragment
#version 440 core

in vec3 v_WorldPos;
in vec3 v_WorldNormal;
in vec2 v_TexCoord;
in mat3 v_TBN;

layout (location = 0) out vec3 g_WorldPos;
layout (location = 1) out vec3 g_WorldNormal;
layout (locatoin = 2) out vec3 g_Albedo;

layout (std430, binding = 3) buffer MaterialStorageBuffer {
    vec4 Albedo;
    vec4 Emissive;
    float Roughness;
    float Metallic;
} ssb_Material;

uniform sampler2D u_AlbedoMap;
uniform sampler2D u_RoughnessMap;
uniform sampler2D u_MetalnessMap;
uniform sampler2D u_NormalMap;
uniform sampler2D u_EmissiveMap;

void main() {
    vec3    texAlbedo       = texture(u_AlbedoMap, v_TexCoord).rgb;
    vec3    texEmissive     = texture(u_EmissiveMap, v_TexCoord).rgb;
    float   texMetallic     = texture(u_MetalnessMap, v_TexCoord).r;
    float   texRoughness    = texture(u_RoughnessMap, v_TexCoord).r;

    vec3    albedo          = pow(texAlbedo, vec3(2.2)) * ssb_Material.Albedo.rgb;
    vec3    emissive        = texEmissive * ssb_Material.Emissive.rgb * ssb_Material.Emissive.a;
    float   metallic        = clamp(texMetallic * ssb_Material.Metallic, 0.0, 1.0);
    float   roughness       = clamp(texRoughness * ssb_Material.Roughness, 0.0, 1.0);

    g_WorldPos = v_WorldPos;
    g_WorldNormal = v_WorldNormal;
    g_Albedo = albedo;
}
