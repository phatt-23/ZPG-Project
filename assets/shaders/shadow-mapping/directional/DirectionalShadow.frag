#type fragment
#version 440 core

in vec4 v_WorldPos;

uniform vec4 u_Color;

void main()
{
    gl_FragDepth = gl_FragCoord.z;
}
