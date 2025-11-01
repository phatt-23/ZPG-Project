#type fragment
#version 440 core

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
    float BeamSize;
    float BeamBlend;
};

layout (std430, binding = 1) buffer LightsStorageBuffer {
    int LightCount;
    Light Lights[];
} ssb_Lights;

layout (std430, binding = 2) buffer CameraStorageBuffer {
    vec3 CameraPos;
} ssb_Camera;



in vec2 v_TexCoord;

uniform sampler2D u_gPosMap;
uniform sampler2D u_gNormalMap;
uniform sampler2D u_gAlbedoMap;
uniform sampler2D u_gEmissiveMap;

out vec4 f_FragColor;

// stolen from learnopengl 
const float PI = 3.14159265359;
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);


void main() {
    // extract material props from maps
    vec3 worldPos = texture(u_gPosMap, v_TexCoord).rgb;
    vec3 worldNormal = texture(u_gNormalMap, v_TexCoord).rgb;

    vec3 albedo = texture(u_gAlbedoMap, v_TexCoord).rgb;
    vec3 emissive = texture(u_gEmissiveMap, v_TexCoord).rgb;

    float metallic = texture(u_gAlbedoMap, v_TexCoord).a;
    float roughness = texture(u_gEmissiveMap, v_TexCoord).a;

    // albedo          = pow(albedo, vec3(2.2));
    emissive        = emissive;
    metallic        = clamp(metallic, 0.0, 1.0);
    roughness       = clamp(roughness, 0.0, 10);


    // f_FragColor = vec4(texture(u_gPosMap, v_TexCoord));
    // f_FragColor = vec4(texture(u_gNormalMap, v_TexCoord));
    // f_FragColor = vec4(texture(u_gAlbedoMap, v_TexCoord));
    // f_FragColor = vec4(texture(u_gEmissiveMap, v_TexCoord));
    // return;


    float ao = 1.0;

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 N = normalize(worldNormal);
    vec3 V = normalize(ssb_Camera.CameraPos - worldPos);

    vec3 Lo = vec3(0.0);
    vec3 La = vec3(0.0);

    // reflectance equation

    for(int i = 0; i < ssb_Lights.LightCount; i++) {

        Light light = ssb_Lights.Lights[i];

        int lightType = light.Type;
        vec3 lightColor = light.Color.rgb * light.Color.a;
        vec3 lightPos = light.Pos.xyz;
        vec3 lightDir = light.Dir.xyz;

        if (lightType == LightTypePoint) {
            // calculate per-light radiance
            vec3 L = normalize(lightPos - worldPos);
            vec3 H = normalize(V + L);
            float distance    = length(lightPos - worldPos);
            float attenuation = 1.0 / (distance * distance);
            vec3 radiance     = lightColor * attenuation;

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
        else if (lightType == LightTypeDirectional) {
            // calculate per-light radiance
            vec3 L = normalize(-lightDir);
            vec3 H = normalize(V + L);
            vec3 radiance     = lightColor;

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
        else if (lightType == LightTypeSpotlight) {
            // calculate per-light radiance
            vec3 L = normalize(lightPos - worldPos);
            vec3 H = normalize(V + L);

            float lightBeamSize = light.BeamSize;
            float lightBeamBlend = light.BeamBlend;

            float beamSizeCos = cos(radians(lightBeamSize));
            float beamBlendCos = cos(radians(lightBeamSize * (1.0 - lightBeamBlend)));

            if (beamBlendCos <= beamSizeCos)
                continue;

            float alpha = dot(normalize(light.Dir), -L);
            float beamNumerator = alpha - beamSizeCos;
            float beamDenominator = beamBlendCos - beamSizeCos;
            float beamContrib = clamp(beamNumerator / beamDenominator, 0.0, 1.0);

            float distance    = length(lightPos - worldPos);
            float attenuation = 1.0 / (distance * distance);
            vec3 radiance     = lightColor * attenuation * beamContrib;

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
        else if (lightType == LightTypeAmbient) {
            La += albedo * lightColor * ao;
        }
    }
    
    vec3 color = Lo + La + emissive;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

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

