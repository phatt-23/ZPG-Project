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
    vec3    texAlbedo       = texture(u_AlbedoMap, v_TexCoord).rgb;

    vec3 Lo = vec3(0.0);

    f_FragColor = vec4(texAlbedo, 1.0);
}
