#type fragment
#version 430 core

const uint LightTypeAmbient      = 1 << 0;
const uint LightTypePoint        = 1 << 1;
const uint LightTypeDirectional  = 1 << 2;
const uint LightTypeSpotlight    = 1 << 3;

struct Light {
    int Type;        // x = Type
    vec4 Color;       // rgba
    vec3 Pos;         // xyz used
    vec3 Dir;         // xyz used
    float BeamSize;  // x = BeamSize, y = BeamBlend
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

in vec3 v_WorldPos;
in vec3 v_WorldNormal;

out vec4 f_FragColor;

void main() {
    vec3 albedo = ssb_Material.Albedo.rgb * ssb_Material.Albedo.a;

    vec3 Lo = vec3(0.0);
    vec3 La = vec3(0.0);

    for (int i = 0; i < ssb_Lights.LightCount; i++) {

        Light light = ssb_Lights.Lights[i];
        vec3 lightColor = light.Color.xyz * light.Color.w;

        vec3 color = lightColor * albedo;

        if (light.Type == LightTypePoint) {
            float dist = length(v_WorldPos - light.Pos);
            float atten = 1.0 / (0.2 * dist*dist + 0.2 * dist + 0.1);

            vec3 V = normalize(ssb_Camera.CameraPos - v_WorldPos);
            vec3 L = normalize(light.Pos - v_WorldPos);
            vec3 N = normalize(v_WorldNormal);
            vec3 H = normalize(L + V);

            vec3 diffuse = max(dot(N,L), 0.0) * color;
            vec3 specular = pow(max(dot(N,H), 0.0), 4 * 16.0) * color;

            Lo += (diffuse + specular) * atten;
        }
        else if (light.Type == LightTypeDirectional) {
            vec3 V = normalize(ssb_Camera.CameraPos - v_WorldPos);
            vec3 L = normalize(-light.Dir);
            vec3 N = normalize(v_WorldNormal);
            vec3 H = normalize(L + V);

            vec3 diffuse = max(dot(N,L), 0.0) * color;
            vec3 specular = pow(max(dot(N,H), 0.0), 4 * 16.0) * color;

            Lo += specular + diffuse;
        }
        else if (light.Type == LightTypeAmbient) {
             La += color;
        }
    }

    f_FragColor = vec4(Lo + La, 1.0) + ssb_Material.Emissive;
}
