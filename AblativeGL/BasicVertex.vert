#version 330

uniform mat4x4 MVP;
in vec3 in_Position;
in vec3 in_Color;
out vec3 ex_Color;
void main(void) {
	gl_Position = MVP * vec4(in_Position, 1.0);
	ex_Color = in_Color;
}