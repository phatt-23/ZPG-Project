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
    vec3 albedo = ssb_Material.Albedo.rgb * ssb_Material.Albedo.a;

    vec3 Lo = vec3(0.0);
    vec3 La = vec3(0.0);

    for (int i = 0; i < ssb_Lights.LightCount; i++) {

        Light light = ssb_Lights.Lights[i];
        vec3 lightColor = light.Color.rgb * light.Color.a;
        vec3 color = lightColor * albedo;

        if (light.Type == LightTypePoint) {
            float dist = length(v_WorldPos - light.Pos);
            float atten = 1.0 / (0.2 * dist*dist + 0.2 * dist + 0.1);

            vec3 V = normalize(ssb_Camera.CameraPos - v_WorldPos);
            vec3 L = normalize(light.Pos - v_WorldPos);
            vec3 N = normalize(v_WorldNormal);
            vec3 R = reflect(-L,N);

            vec3 diffuse = max(dot(N,L), 0.0) * color;

            Lo += diffuse * atten;
        }
        else if (light.Type == LightTypeDirectional) {
            vec3 V = normalize(ssb_Camera.CameraPos - v_WorldPos);
            vec3 L = normalize(-light.Dir);
            vec3 N = normalize(v_WorldNormal);
            vec3 R = reflect(-L,N);

            vec3 diffuse = max(dot(L,N), 0.0) * color;

            Lo += diffuse;
        }
        else if (light.Type == LightTypeAmbient) {
            La += color;
        }
    }

    f_FragColor = vec4(Lo + La, 1.0) + ssb_Material.Emissive;
}
