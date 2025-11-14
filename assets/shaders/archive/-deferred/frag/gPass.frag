#type fragment
#version 440 core

in vec3 v_WorldPos;
in vec3 v_WorldNormal;
in vec2 v_TexCoord;
in mat3 v_TBN;
in flat int v_EntityID;

layout (location = 0) out vec4 g_Color0;
layout (location = 1) out vec4 g_Color1;
layout (location = 2) out vec4 g_Color2;
layout (location = 3) out vec4 g_Color3;
layout (location = 4) out int g_Color4;

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

    g_Color0.rgb = v_WorldPos;
    g_Color0.a = 1.0;

    g_Color1.rgb = N;
    g_Color1.a = 1.0;

    g_Color2.rgb = albedo;
    g_Color2.a = metallic;

    g_Color3.rgb = emissive;
    g_Color3.a = roughness;

    g_Color4.r = v_EntityID;

    /**
        The alpha chanell is still active, even in the deferred pass.
        Normally what happends is that gl_FragColor is written into.
        gl_FragColor = vec4(r,g,b,a)

        This will automatically multiply the alpha component with the rgb components.

        (r,g,b) * a

        Thus the r,g and b components are manipulate with implicitly.

        If I want to just store the data, the alpha has to be disabled.
    */
}
