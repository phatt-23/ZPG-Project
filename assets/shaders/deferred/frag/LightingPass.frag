#type fragment
#version 440 core

// enum of light types
const uint LightTypeAmbient      = 1 << 0;
const uint LightTypePoint        = 1 << 1;
const uint LightTypeDirectional  = 1 << 2;
const uint LightTypeSpotlight    = 1 << 3;

struct Light {
    vec3 Pos;
    int Type;
    vec4 Color;
    vec3 Dir;
    float BeamSize;
    vec3 Atten;
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

uniform samplerCube u_SkyboxMap;
uniform sampler2D g_Color0; // pos
uniform sampler2D g_Color1; // normal
uniform sampler2D g_Color2; // albedo and metallic
uniform sampler2D g_Color3; // emissive and roughness
uniform isampler2D g_Color4; // entityID

layout(location = 0) out vec4 f_Color0;
layout(location = 1) out int f_Color1;

// stolen from learnopengl 
const float PI = 3.14159265359;
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

void main()
{
    // extract material props from maps
    vec3 worldPos = texture(g_Color0, v_TexCoord).rgb;
    vec3 worldNormal = texture(g_Color1, v_TexCoord).rgb;

    vec3 albedo = texture(g_Color2, v_TexCoord).rgb;
    float metallic = texture(g_Color2, v_TexCoord).a;

    vec3 emissive = texture(g_Color3, v_TexCoord).rgb;
    float roughness = texture(g_Color3, v_TexCoord).a;

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
        vec3 lightAtten = light.Atten;

        if (lightType == LightTypePoint) {
            // calculate per-light radiance
            vec3 L = normalize(lightPos - worldPos);
            vec3 H = normalize(V + L);
            float distance    = length(lightPos - worldPos);
            float attenuation = min(1.0 / (lightAtten.x * distance * distance + lightAtten.y * distance + lightAtten.z), 1.0);
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
            float attenuation = min(1.0 / (lightAtten.x * distance * distance + lightAtten.y * distance + lightAtten.z), 1.0);
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

    vec3 R = reflect(-V, N);
    vec3 envColor = texture(u_SkyboxMap, R).rgb; // sample environment reflection

    vec3 F = fresnelSchlick(max(dot(N, V), 0.0), F0); // Fresnel term — stronger near grazing angles

    float reflectionStrength = (1.0 - roughness);
    vec3 reflection = envColor * F * reflectionStrength; // Attenuate reflection by smoothness (1 - roughness)

    vec3 finalColor = color + reflection; // Combine lighting with reflection

    // Tone mapping + gamma correction
    finalColor = finalColor / (finalColor + vec3(1.0));
    finalColor = pow(finalColor, vec3(1.0 / 2.2));

    f_Color0 = vec4(finalColor, 1.0);
    f_Color1 = texture(g_Color4, v_TexCoord).r; // should be read as an int
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


