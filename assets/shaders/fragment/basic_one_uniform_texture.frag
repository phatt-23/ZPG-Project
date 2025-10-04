#type fragment
#version 400

in vec3 v_WorldPos;
in vec3 v_WorldNormal;
in vec2 v_TexCoord;

uniform sampler2D u_Texture;

out vec4 f_FragColor;

void main() {
    vec4 texColor = texture(u_Texture, v_TexCoord);

    // f_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
    f_FragColor = texColor;
}
