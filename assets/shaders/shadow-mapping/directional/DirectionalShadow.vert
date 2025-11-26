#type vertex
#version 440 core

#include "ext/ssbo/EnvironmentLightSSBO.glsl"
#include "ext/ssbo/ModelSSBO.glsl"

layout(location = 0) in vec3 a_Pos; // only need the objects position

void main()
{
    // mat4 model = ssbo_Model.Models[gl_InstanceID];
    // mat4 lightViewProj = ssbo_EnvironmentLight.DirLight.ViewProj[0];
    //
    // v_WorldPos = model * vec4(a_Pos, 1.0);
    // gl_Position = lightViewProj * v_WorldPos;

    mat4 model = ssbo_Model.Models[gl_InstanceID];
    gl_Position = model * vec4(a_Pos, 1.0);
}
