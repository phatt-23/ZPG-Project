#type fragment
#version 400

uniform vec4 u_Albedo;

out vec4 f_FragColor;

void main() {
    f_FragColor = u_Albedo;
}
