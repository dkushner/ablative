#version 330 core

out vec4 out_Color;
in vec3 color;

void main (void)
{
	out_Color = vec4(color.xyz, 1.0);
}