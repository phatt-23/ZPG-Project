#type geometry
#version 440 core

#include "ext/ssbo/EnvironmentLightSSBO.glsl"

layout(triangles, invocations = 3) in; // cascade count = 3
layout(triangle_strip, max_vertices=3) out;

void main()
{
    DirectionalLight light = ssbo_EnvironmentLight.DirLight;

    // for (int cascade = 0; cascade < light.CascadeCount; cascade++)
    // {

    for (int i = 0; i < 3; i++)    
    {
        gl_Position = light.ViewProj[gl_InvocationID] * gl_in[i].gl_Position;             
        gl_Layer = gl_InvocationID;
        EmitVertex();
    }
    EndPrimitive();

    // }
}
