#type fragment
#version 400

// enum of light types
const uint LightTypeAmbient      = 0b0001;
const uint LightTypePoint        = 0b0010;
const uint LightTypeDirectional  = 0b0100;
const uint LightTypeSpotlight    = 0b1000;

struct Light {
    int type;
    vec3 color;
    vec3 pos;
    vec3 dir;
    float inCutoff;
    float outCutoff;
};

uniform Light u_Lights[100];
uniform int u_LightCount;
uniform vec3 u_AmbientColor;
uniform vec3 u_CameraPos;

in vec3 v_WorldPos;
in vec3 v_WorldNormal;
in vec2 v_TexCoord;

out vec4 f_FragColor;

void main() {

    // calculate the final light color
    int count = min(u_LightCount, u_Lights.length());

    for (int i = 0; i < count; i++) {
        Light light = u_Lights[i];
        
        if (light.type == LightTypeDirectional) {
            
        } else if (light.type == LightTypePoint) {
            
        } else if (light.type == LightTypeSpotlight) {
            
        }
    }

    f_FragColor = vec4(v_WorldNormal, 1.f);
}