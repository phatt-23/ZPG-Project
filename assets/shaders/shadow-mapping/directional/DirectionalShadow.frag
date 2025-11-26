#type fragment
#version 440 core

void main()
{
    gl_FragDepth = gl_FragCoord.z;
}
