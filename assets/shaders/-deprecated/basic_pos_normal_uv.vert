#type vertex
#version 400

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

out vec3 v_WorldPos;
out vec3 v_WorldNormal;
out vec2 v_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_ViewProj;

void main() {
    vec4 worldPos = u_Model * vec4(a_Pos, 1.f);
    v_WorldPos = vec3(worldPos);
    v_WorldNormal = normalize(transpose(inverse(mat3(u_Model))) * a_Normal);
    v_TexCoord = a_TexCoord;
    gl_Position = u_ViewProj * worldPos;
}
