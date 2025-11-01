#type fragment
#version 440 core

in vec3 v_WorldPos;
in vec3 v_WorldNormal;
in vec2 v_TexCoord;
in mat3 v_TBN;

layout (location = 0) out vec4 g_Pos;
layout (location = 1) out vec4 g_Normal;
layout (location = 2) out vec4 g_Albedo;
layout (location = 3) out vec4 g_Emissive;
layout (location = 4) out vec4 g_MetallicRougness;

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
    float   texMetallic     = texture(u_MetalnessMap, v_TexCoord).b;
    float   texRoughness    = texture(u_RoughnessMap, v_TexCoord).g;
    vec3    texNormal       = texture(u_NormalMap, v_TexCoord).rgb;

    vec3    albedo          = pow(texAlbedo, vec3(2.2)) * ssb_Material.Albedo.rgb;
    vec3    emissive        = texEmissive * ssb_Material.Emissive.rgb * ssb_Material.Emissive.a;
    float   metallic        = clamp(texMetallic * ssb_Material.Metallic, 0.0, 1.0);
    float   roughness       = clamp(texRoughness * ssb_Material.Roughness, 0.0, 1.0);

    vec3 tangentNormal = texture(u_NormalMap, v_TexCoord).rgb * 2.0 - 1.0;
    vec3 N = normalize(v_TBN * tangentNormal);

    g_Pos.rgb = v_WorldPos;
    g_Pos.a = 1.0;

    // rgba
    g_Normal.rgb = N;
    g_Normal.a = 1.0;

    g_Albedo.rgb = albedo;
    g_Albedo.a = metallic;

    // g_AlbedoMetallic.a      = metallic;
    g_Emissive.rgb = emissive;
    g_Emissive.a = roughness;

    /**
    The alpha chanell is still active, even in the deferred pass.
    Normally what happends is that gl_FragColor is written into.
    gl_FragColor = vec4(r,g,b,a)

    This will automatically multiply the alpha component with the rgb components.

    (r,g,b) * a

    Thus the r,g and b components are manipulate with implicitly.

    If I want to just store the data, the alpha has to be disabled.

    */

    // g_EmissiveRoughness.a   = roughness;

//    g_Pos               = vec4(v_WorldPos,      1.0);
//    g_Normal            = vec4(v_WorldNormal,   1.0);
//    g_AlbedoMetallic    = vec4(albedo,          metallic);
//    g_EmissiveRoughness = vec4(emissive,        roughness);
}
