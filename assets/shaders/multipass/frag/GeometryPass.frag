#type fragment
#version 440 core

in vec4 v_WorldPos;
in vec3 v_WorldNormal;
in vec2 v_TexCoord;
in mat3 v_TBN;
in flat int v_EntityID;

layout(location = 0) out vec4 g_Color0;
layout(location = 1) out vec4 g_Color1;
layout(location = 2) out vec4 g_Color2;
layout(location = 3) out vec4 g_Color3;
layout(location = 4) out int g_Color4;
layout(location = 5) out vec4 g_Color5;

layout (std430, binding = 4) buffer MaterialStorageBuffer
{
    vec4 Albedo;
    vec4 Emissive;
    float Roughness;
    float Metallic;
} ssbo_Material;

uniform sampler2D u_AlbedoMap;
uniform sampler2D u_RoughnessMap;
uniform sampler2D u_MetalnessMap;
uniform sampler2D u_NormalMap;
uniform sampler2D u_EmissiveMap;

void main()
{
    vec3 texAlbedo = texture(u_AlbedoMap, v_TexCoord).rgb;
    vec3 texEmissive = texture(u_EmissiveMap, v_TexCoord).rgb;
    float texMetallic  = texture(u_MetalnessMap, v_TexCoord).b;
    float texRoughness = texture(u_RoughnessMap, v_TexCoord).g;
    vec3 texNormal = texture(u_NormalMap, v_TexCoord).rgb;

    vec3 albedo = texAlbedo * ssbo_Material.Albedo.rgb;
    vec3 emissive = texEmissive * ssbo_Material.Emissive.rgb * ssbo_Material.Emissive.a;
    float metallic = clamp(texMetallic * ssbo_Material.Metallic, 0.0, 1.0);
    float roughness = clamp(texRoughness * ssbo_Material.Roughness, 0.0, 1.0);

    vec3 tangentNormal = texture(u_NormalMap, v_TexCoord).rgb * 2.0 - 1.0;
    vec3 N = normalize(v_TBN * tangentNormal);

    g_Color0 = v_WorldPos;

    // g_Color1.rgb = N;
    g_Color1.rgb = N * 0.5 + 0.5;
    g_Color1.a = 1.0;

    g_Color2.rgb = albedo;
//    g_Color2.a = 1.0;
     g_Color2.a = metallic;

    g_Color3.rgb = emissive;
//    g_Color3.a = 1.0;
     g_Color3.a = roughness;

    g_Color4.r = v_EntityID;
    g_Color5 = vec4(vec3(v_EntityID), 1.0);
}
