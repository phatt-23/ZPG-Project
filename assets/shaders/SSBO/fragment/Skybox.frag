#type fragment
#version 440 core

in vec3 v_TexCoords;
uniform samplerCube u_SkyboxMap;
out vec4 f_FragColor;

void main() {
    vec3 col = texture(u_SkyboxMap, v_TexCoords).rgb;
    f_FragColor = vec4(col, 1.0); // ensure alpha or packed metalness as needed
}

