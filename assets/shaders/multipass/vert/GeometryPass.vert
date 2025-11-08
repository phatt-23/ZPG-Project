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

layout (std430, binding = 0) buffer CameraShaderStorageBuffer
{
    mat4 View;
    mat4 Proj;
    mat4 ViewProj;
    vec3 CameraPosition;
} ssbo_Camera;

layout (std430, binding = 5) buffer ModelShaderStorageBuffer
{
    int ModelCount;
    mat4 Models[];
} ssbo_Model;

layout (std430, binding = 6) buffer EntityShaderStorageBuffer
{
    int EntityCount;
    float pad[3];
    int EntityIDs[];
} ssbo_Entity;

void main() {
    mat4 model = ssbo_Model.Models[gl_InstanceID];

    vec4 worldPos = model * vec4(a_Pos, 1.f);

    v_EntityID = ssbo_Entity.EntityIDs[gl_InstanceID];

    mat3 normalMat = transpose(inverse(mat3(model)));

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
