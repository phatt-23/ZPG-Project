#type fragment
#version 430

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

uniform sampler2D u_AlbedoMap;
uniform sampler2D u_RoughnessMap;
uniform sampler2D u_MetalnessMap;
uniform sampler2D u_NormalMap;

in vec3 v_WorldPos;
in vec3 v_WorldNormal;

out vec4 f_FragColor;

void main() {
    vec3 albedo = u_Material.Albedo.rgb * u_Material.Albedo.a;

    vec3 Lo = vec3(0.0);
    vec3 La = vec3(0.0);

    for (int i = 0; i < u_Lights.LightCount; i++) {

        Light light = u_Lights.Lights[i];
        vec3 lightColor = light.Color.rgb * light.Color.a;
        vec3 color = lightColor * albedo;

        if (light.Type == LightTypePoint) {
            float dist = length(v_WorldPos - light.Pos);
            float atten = 1.0 / (0.2 * dist*dist + 0.2 * dist + 0.1);

            vec3 V = normalize(u_Camera.CameraPos - v_WorldPos);
            vec3 L = normalize(light.Pos - v_WorldPos);
            vec3 N = normalize(v_WorldNormal);
            vec3 H = normalize(L + V);

            vec3 diffuse = max(dot(N,L), 0.0) * color;
            vec3 specular = pow(max(dot(N,H), 0.0), 4 * 16.0) * color;

            Lo += (diffuse + specular) * atten;
        }
        else if (light.Type == LightTypeDirectional) {
            vec3 V = normalize(u_Camera.CameraPos - v_WorldPos);
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

    f_FragColor = vec4(Lo + La, 1.0) + u_Material.Emissive;
}
