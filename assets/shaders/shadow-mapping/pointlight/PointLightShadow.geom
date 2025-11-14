#type geometry
#version 440 core

layout(triangles) in;
layout(triangle_strip, max_vertices=18) out;

#include "ext/ssbo/PointLightSSBO.glsl"

uniform int u_Index;

in vec4 v_WorldPos[];
out vec4 g_WorldPos;

void main()
{
    PointLight light = ssbo_PointLight.LightArray[u_Index];

    for (int f = 0; f < 6; ++f)
    {
        gl_Layer = (6 * u_Index) + f;
        for (int v = 0; v < 3; ++v)
        {
            g_WorldPos = v_WorldPos[v];
            gl_Position = light.ViewProj[f] * g_WorldPos;
            EmitVertex();
        }
        EndPrimitive();
    }
}
