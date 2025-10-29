#type fragment
#version 440 core

in vec2 v_TexCoord;

uniform sampler2D u_gPositionMap;
uniform sampler2D u_gNormalMap;
uniform sampler2D u_gAlbedoMap;

out vec4 f_FragColor;

void main() {
    vec3 pos = texture(u_gPositionMap, v_TexCoord).rgb;
    vec3 N = texture(u_gNormalMap, v_TexCoord).rgb;
    vec3 albedo = texture(u_gAlbedoMap, v_TexCoord).rgb;

    f_FragColor = texture(u_gAbledoMap, v_TexCoord).rgba;
}