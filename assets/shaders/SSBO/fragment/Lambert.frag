#type fragment
#version 430 core

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


in vec3 v_WorldPos;
in vec3 v_WorldNormal;

out vec4 f_FragColor;

void main() {
    vec3 albedo = ssb_Material.Albedo.rgb;
    vec3 emissive = ssb_Material.Emissive.rgb;

    vec3 Lo = vec3(0.0);
    vec3 La = vec3(0.0);

    vec3 N = normalize(v_WorldNormal);
    vec3 V = normalize(ssb_Camera.CameraPos - v_WorldPos);

    for (int i = 0; i < ssb_Lights.LightCount; i++) {
        Light light = ssb_Lights.Lights[i];
        vec3 lightColor = light.Color.rgb * light.Color.a;

        if (light.Type == LightTypePoint) {
            vec3 L = normalize(light.Pos - v_WorldPos);
            float dist = length(light.Pos - v_WorldPos);

            float atten = 1.0 / (1.0 + 0.22 * dist + 0.20 * dist * dist);

            float NdotL = max(dot(N, L), 0.0);

            vec3 diffuse = NdotL * albedo * lightColor;

            Lo += diffuse * atten;
        }
        else if (light.Type == LightTypeDirectional) {
            vec3 L = normalize(-light.Dir);
            float NdotL = max(dot(N, L), 0.0);

            vec3 diffuse = NdotL * albedo * lightColor;

            Lo += diffuse;
        }
        else if (light.Type == LightTypeAmbient) {
            La += albedo * lightColor;
        }
    }

    vec3 color = Lo + La + emissive;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    f_FragColor = vec4(color, 1.0);
}
