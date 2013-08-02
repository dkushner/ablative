#version 330 core


precision highp float;
out vec4 out_Color;
in float light;

void main (void)
{
	out_Color = vec4(0.6*light, 0.6*light, 0.6*light, 1.0);
}