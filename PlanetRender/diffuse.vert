#version 330 core

uniform mat4 MVP;

in vec3 in_Position;
in vec3 in_Normal;
out float light;

void main()
{
	vec3 lightPos = vec3(1, 1, 1);
	vec4 surfaceNormal = MVP * vec4(in_Normal, 0.0);
	light = max(dot(surfaceNormal.xyz, normalize(lightPos)), 0.0);
	gl_Position = MVP * vec4(in_Position, 1.0);
}