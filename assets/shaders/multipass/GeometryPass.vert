#type vertex
#version 440 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Tangent;

out vec4 v_WorldPos;
out vec3 v_WorldNormal;
out vec2 v_TexCoord;
out mat3 v_TBN;
out flat int v_EntityID;

#include "ext/ssbo/CameraSSBO.glsl"
#include "ext/ssbo/ModelSSBO.glsl"
#include "ext/ssbo/EntitySSBO.glsl"

void main()
{
    mat4 model = ssbo_Model.Models[gl_InstanceID];

    vec4 worldPos = model * vec4(a_Pos, 1.f);
    mat3 normalMat = transpose(inverse(mat3(model)));

    v_EntityID = ssbo_Entity.EntityIDs[gl_InstanceID];
    v_WorldPos = worldPos;
    v_WorldNormal = normalize(normalMat * a_Normal);
    v_TexCoord = a_TexCoord;

    vec3 N = v_WorldNormal;
    vec3 T = normalize(normalMat * a_Tangent);
    T = normalize(T - dot(T, N) * N);
    vec3 B = normalize(cross(N, T));

    v_TBN = mat3(T, B, N);

    gl_Position = ssbo_Camera.ViewProj * worldPos;
}
