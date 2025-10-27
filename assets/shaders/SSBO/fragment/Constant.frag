#type fragment
#version 430 core

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

in vec3 v_WorldPos;
in vec3 v_WorldNormal;
in vec2 v_TexCoord;
in mat3 v_TBN;

out vec4 f_FragColor;

void main() {
    vec3 albedo = texture(u_AlbedoMap, v_TexCoord).rgb * ssb_Material.Albedo.rgb;

    f_FragColor = vec4(albedo, 1.0);
}
