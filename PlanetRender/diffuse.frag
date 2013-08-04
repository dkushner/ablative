#version 330 core

out vec4 out_Color;
in float light;


void main (void)
{
	out_Color = vec4(light, light, light, 1.0);
}