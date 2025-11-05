#type fragment
#version 440 core

in vec2 v_TexCoord;

out vec4 f_FragColor;

uniform sampler2D u_SkydomeMap;

void main()
{
   f_FragColor = texture(u_SkydomeMap, v_TexCoord);
}
