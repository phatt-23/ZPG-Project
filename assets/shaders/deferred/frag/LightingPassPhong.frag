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

    vec3    diffuseColor    = albedo * clamp(1.0 - metallic, 0.01, 1.0);
    float   shininess       = max(pow(1.0 - roughness, 4.0) * 512.0, 16.0);
    vec3    baseSpecColor   = mix(vec3(0.04), albedo, metallic);
    float   specIntensity   = mix(0.5, 2.0, metallic) * mix(0.2, 1.0, pow(1.0 - roughness + 0.001, 2.0));
    vec3    specularColor   = baseSpecColor * specIntensity;


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

    for (int i = 0; i < ssb_Lights.LightCount; i++) {
        Light light = ssb_Lights.Lights[i];
        vec3 lightColor = light.Color.rgb * light.Color.a;
        int lightType = light.Type;
        vec3 lightPos = light.Pos;
        vec3 lightDir = light.Dir;

        if (lightType == LightTypePoint) {
            vec3 L = normalize(lightPos - worldPos);
            vec3 R = reflect(-L, N);
            float dist = length(lightPos - worldPos);
            float atten = 1.0 / (1.0 + 0.22 * dist + 0.20 * dist * dist); 

            vec3 diffuse = max(dot(N, L), 0.0) * diffuseColor;
            vec3 specular = pow(max(dot(V, R), 0.0), shininess) * specularColor;

            Lo += (diffuse + specular) * lightColor * atten;
        }
        else if (lightType == LightTypeDirectional) {
            vec3 L = normalize(-lightDir);
            vec3 R = reflect(-L, N);

            vec3 diffuse = max(dot(N, L), 0.0) * diffuseColor;
            vec3 specular = pow(max(dot(V, R), 0.0), shininess) * specularColor;

            Lo += (diffuse + specular) * lightColor;
        }
        else if (lightType == LightTypeSpotlight) {
            vec3 L = normalize(lightPos - worldPos);
            vec3 R = reflect(-L, N);
            float dist = length(light.Pos - worldPos);
            float atten = 1.0 / (1.0 + 0.22 * dist + 0.20 * dist * dist);

            float lightBeamSize = light.BeamSize;
            float lightBeamBlend = light.BeamBlend;

            float beamSizeCos = cos(radians(lightBeamSize));
            float beamBlendCos = cos(radians(lightBeamSize * (1.0 - lightBeamBlend)));

            if (beamBlendCos <= beamSizeCos) continue;

            float alpha = dot(normalize(light.Dir), -L);
            float beamNumerator = alpha - beamSizeCos;
            float beamDenominator = beamBlendCos - beamSizeCos;
            float beamContrib = clamp(beamNumerator / beamDenominator, 0.0, 1.0);

            vec3 diffuse = max(dot(N, L), 0.0) * diffuseColor;
            vec3 specular = pow(max(dot(V, R), 0.0), shininess) * specularColor;

            Lo += (diffuse + specular) * lightColor * beamContrib * atten;
        }
        else if (lightType == LightTypeAmbient) {
            La += albedo * lightColor;
        }
    }
    
    vec3 color = Lo + La + emissive;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    f_FragColor = vec4(color, 1.0);
}
