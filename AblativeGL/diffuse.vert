#version 330 core

uniform mat4 MVP;

in vec3 in_Position;
in vec3 in_Normal;
out vec3 color;

void main()
{
	//vec3 lightPos = vec3(1, 0.3, 0.3);
	//vec4 surfaceNormal = MVP * vec4(in_Normal, 0.0);
	//light = max(dot(surfaceNormal.xyz, normalize(lightPos)), 0.0);
	color = in_Normal;
	gl_Position = MVP * vec4(in_Position, 1.0);
}