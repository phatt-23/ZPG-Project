#type vertex
#version 440 core

#include "ext/ssbo/CameraSSBO.glsl"
#include "ext/ssbo/ModelSSBO.glsl"
#include "ext/ssbo/EntitySSBO.glsl"

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Tangent;

out vec3 v_WorldPos;
out vec3 v_WorldNormal;
out vec2 v_TexCoord;
out mat3 v_TBN;
out flat int v_EntityID;

void main() {
    mat4 model = ssbo_Model.Models[gl_InstanceID];
    int entityID = ssbo_Entity.EntityIDs[gl_InstanceID];

    vec4 worldPos = model * vec4(a_Pos, 1.0);
    mat3 normalMat = transpose(inverse(mat3(model)));

    vec3 normal = normalize(normalMat * a_Normal);
    vec3 N = normal;
    vec3 T = normalize(normalMat * a_Tangent);
    T = normalize(T - dot(T, N) * N);
    vec3 B = normalize(cross(N, T));

    v_TBN = mat3(T,B,N);
    v_WorldPos = worldPos.xyz / worldPos.w;
    v_WorldNormal = normalMat * a_Normal;
    v_TexCoord = a_TexCoord;
    v_EntityID = entityID;

    gl_Position = ssbo_Camera.ViewProj * worldPos;
}
