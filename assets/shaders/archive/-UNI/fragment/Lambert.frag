#type fragment
#version 400

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

uniform Light u_Lights[100];
uniform int u_LightCount;
uniform vec3 u_CameraPos;
uniform vec4 u_AmbientColor;

uniform vec4 u_Albedo;
uniform vec4 u_Emissive;

in vec3 v_WorldPos;
in vec3 v_WorldNormal;

out vec4 f_FragColor;

void main() {
    vec3 albedo = u_Albedo.rgb * u_Albedo.a;
    vec3 Lo = vec3(0.0);

    for (int i = 0; i < u_LightCount; i++) {

        Light light = u_Lights[i];
        vec3 lightColor = light.Color.rgb * light.Color.a;
        vec3 color = lightColor * albedo;

        if (light.Type == LightTypePoint) {
            float dist = length(v_WorldPos - light.Pos);
            float atten = 1.0 / (0.2 * dist*dist + 0.2 * dist + 0.1);

            vec3 V = normalize(u_CameraPos - v_WorldPos);
            vec3 L = normalize(light.Pos - v_WorldPos);
            vec3 N = normalize(v_WorldNormal);
            vec3 R = reflect(-L,N);

            vec3 diffuse = max(dot(N,L), 0.0) * color;

            Lo += diffuse * atten;
        }
        else if (light.Type == LightTypeDirectional) {
            vec3 V = normalize(u_CameraPos - v_WorldPos);
            vec3 L = normalize(-light.Dir);
            vec3 N = normalize(v_WorldNormal);
            vec3 R = reflect(-L,N);

            vec3 diffuse = max(dot(L,N), 0.0) * color;

            Lo += diffuse;
        }
    }

    vec3 ambient = u_AmbientColor.xyz * u_AmbientColor.w;
    vec3 La = ambient * albedo;
    f_FragColor = vec4(Lo + La, 1.0) + u_Emissive;
}
