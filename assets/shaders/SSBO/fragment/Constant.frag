#type fragment
#version 430 core

layout (std430, binding = 3) buffer MaterialStorageBuffer {
    vec4 Albedo;
    vec4 Emissive;
    float Roughness;
    float Metallic;
} ssb_Material;

out vec4 f_FragColor;

void main() {
    f_FragColor = ssb_Material.Albedo;
}
