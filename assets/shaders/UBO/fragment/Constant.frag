#type fragment
#version 420

layout (std140, binding = 3) uniform MaterialUBO {
    vec4 Albedo;
    float Roughness;
    float Metallic;
} u_Material;

out vec4 f_FragColor;

void main() {
    f_FragColor = u_Material.Albedo + vec4(0.1, 0.1, 0.1, 1.0);
}
