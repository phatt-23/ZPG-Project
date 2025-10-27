#type fragment
#version 330

in vec3 v_WorldNormal;
out vec4 f_Color;

void main() {
    f_Color = vec4(v_WorldNormal, 1.0);
}
