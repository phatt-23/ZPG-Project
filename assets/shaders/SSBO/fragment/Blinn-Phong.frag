#type fragment
#version 430 core

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
    vec3    texEmissive     = texture(u_EmissiveMap, v_TexCoord).rgb;
    float   texMetallic     = texture(u_MetalnessMap, v_TexCoord).r;
    float   texRoughness    = texture(u_RoughnessMap, v_TexCoord).r;

    vec3    albedo          = pow(texAlbedo, vec3(2.2)) * ssb_Material.Albedo.rgb;
    vec3    emissive        = texEmissive * ssb_Material.Emissive.rgb * ssb_Material.Emissive.a;
    float   metallic        = clamp(texMetallic * ssb_Material.Metallic, 0.0, 1.0);
    float   roughness       = clamp(texRoughness * ssb_Material.Roughness, 0.0, 1.0);

    // Metals - less diffuse, more specular and colored reflection
    vec3    diffuseColor    = albedo * clamp(1.0 - metallic, 0.01, 1.0);

    // Roughness to shininess (higher roughness, lower shininess)
    float   shininess       = max(pow(1.0 - roughness, 4.0) * 512.0, 16.0);

    // Intensity adjustment:
    //   metals - higher reflectivity
    //   rough surfaces - broader and dimmer highlights
    vec3    baseSpecColor   = mix(vec3(0.04), albedo, metallic);
    float   specIntensity   = mix(0.5, 2.0, metallic) * mix(0.2, 1.0, pow(1.0 - roughness + 0.001, 2.0));
    vec3    specularColor   = baseSpecColor * specIntensity;


    vec3 Lo = vec3(0.0);
    vec3 La = vec3(0.0);
    vec3 Le = texture(u_EmissiveMap, v_TexCoord).rgb * ssb_Material.Emissive.rgb * ssb_Material.Emissive.a;

    vec3 tangentNormal  = 2.0 * texture(u_NormalMap, v_TexCoord).rgb - 1.0;
    vec3 N              = normalize(v_TBN * tangentNormal);

    vec3 V = normalize(ssb_Camera.CameraPos - v_WorldPos);

    for (int i = 0; i < ssb_Lights.LightCount; i++) {
        Light light = ssb_Lights.Lights[i];
        vec3 lightColor = light.Color.rgb * light.Color.a;
        int lightType = light.Type;
        vec3 lightPos = light.Pos;
        vec3 lightDir = light.Dir;

        if (lightType == LightTypePoint) {
            vec3 L = normalize(lightPos - v_WorldPos);
            vec3 H = normalize(L + V);
            float dist = length(lightPos - v_WorldPos);
            float atten = 1.0 / (1.0 + 0.22 * dist + 0.20 * dist * dist);

            float NdotL = max(dot(N, L), 0.0);
            float NdotH = max(dot(N, H), 0.0);

            vec3 diffuse = NdotL * diffuseColor;
            vec3 specular = pow(NdotH, shininess) * specularColor;

            Lo += (diffuse + specular) * lightColor * atten;
        }
        else if (lightType == LightTypeDirectional) {
            vec3 L = normalize(-lightDir);
            vec3 H = normalize(L + V);

            float NdotL = max(dot(N, L), 0.0);
            float NdotH = max(dot(N, H), 0.0);

            vec3 diffuse = NdotL * diffuseColor;
            vec3 specular = pow(NdotH, shininess) * specularColor;

            Lo += (diffuse + specular) * lightColor;
        }
        else if (lightType == LightTypeSpotlight) {
            vec3 L = normalize(lightPos - v_WorldPos);
            vec3 H = normalize(V + L);
            float dist = length(light.Pos - v_WorldPos);
            float atten = 1.0 / (1.0 + 0.22 * dist + 0.20 * dist * dist);

            float lightBeamSize = light.BeamSize;
            float lightBeamBlend = max(light.BeamBlend, 0.01);

            float beamSizeCos = cos(radians(lightBeamSize));
            float beamBlendCos = cos(radians(lightBeamSize * (1.0 - lightBeamBlend)));

            if (beamBlendCos <= beamSizeCos) continue;

            float alpha = dot(normalize(light.Dir), -L);
            float beamNumerator = alpha - beamSizeCos;
            float beamDenominator = beamBlendCos - beamSizeCos;
            float beamContrib = clamp(beamNumerator / beamDenominator, 0.0, 1.0);


            float NdotL = max(dot(N, L), 0.0);
            float NdotH = max(dot(N, H), 0.0);

            vec3 diffuse = NdotL * diffuseColor;
            vec3 specular = pow(NdotH, shininess) * specularColor;

            Lo += (diffuse + specular) * lightColor * beamContrib * atten;
        }
        else if (lightType == LightTypeAmbient) {
            La += albedo * lightColor;
        }
    }

    vec3 color = Lo + La + Le;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    f_FragColor = vec4(color, 1.0);
}
