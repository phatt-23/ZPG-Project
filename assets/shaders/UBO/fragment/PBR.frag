#type fragment
#version 420 core

// later add this feature
// #include "Lights.glsl"

// enum of light types
const uint LightTypeAmbient      = 1 << 0;
const uint LightTypePoint        = 1 << 1;
const uint LightTypeDirectional  = 1 << 2;
const uint LightTypeSpotlight    = 1 << 3;

struct Light {
    int Type;
    vec4 Color;
    vec3 Pos;
    vec3 Dir;
    float InCutoff;
    float OutCutoff;
};

layout (std140, binding = 1) uniform LightsUBO {
    Light Lights[100];
    int LightCount;
} u_Lights;

layout (std140, binding = 2) uniform CameraUBO {
    vec3 CameraPos;
} u_Camera;

layout (std140, binding = 3) uniform MaterialUBO {
    vec4 Albedo;
    vec4 Emissive;
    float Roughness;
    float Metallic;
} u_Material;

in vec3 v_WorldPos;
in vec3 v_WorldNormal;

out vec4 f_FragColor;

const float PI = 3.14159265359;

// stolen from learnopengl 
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

void main() {
    vec3 albedo = u_Material.Albedo.rgb / u_Material.Albedo.a;
    float ao = 1.0;
    vec3 N = normalize(v_WorldNormal);
    vec3 V = normalize(u_Camera.CameraPos - v_WorldPos);
    float metallic = max(u_Material.Metallic, 0.07);
    float roughness = max(u_Material.Roughness, 0.07);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);

    for(int i = 0; i < u_Lights.LightCount; i++) {
//        if (u_Lights[i].type != LightTypePoint) {
//            continue;
//        }

        // calculate per-light radiance
        vec3 L = normalize(u_Lights.Lights[i].Pos - v_WorldPos);
        vec3 H = normalize(V + L);
        float distance    = length(u_Lights.Lights[i].Pos - v_WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance     = vec3(u_Lights.Lights[i].Color * attenuation);

        // cook-torrance brdf
        float NDF = DistributionGGX(N, H, roughness);
        float G   = GeometrySmith(N, V, L, roughness);
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular     = numerator / denominator;

        // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

    f_FragColor = vec4(color, 1.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
