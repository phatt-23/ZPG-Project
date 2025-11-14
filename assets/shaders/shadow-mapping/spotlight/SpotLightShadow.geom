#type geometry
#version 440 core

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

uniform int u_Index;

void main()
{
    for (int i = 0; i < 3; i++)
    {
        gl_Layer = u_Index;
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
