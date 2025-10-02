#type fragment
#version 330

in vec3 v_Normal;
out vec4 f_Color;

void main() {
    f_Color = vec4(v_Normal, 1.f);
}