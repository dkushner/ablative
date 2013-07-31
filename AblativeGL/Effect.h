#ifndef EFFECT_H
#define EFFECT_H

#include <GL/glew.h>
#include <string>

using namespace std;

class Effect
{
	public:
		Effect(){}
		Effect(const Effect& effect)
		{
			this->fragment_shader = effect.fragment_shader;
			this->geometry_shader = effect.geometry_shader;
			this->vertex_shader = effect.vertex_shader;
			this->program = effect.program;
		}
		template <int V, int F>
		Effect(GLchar const* (&vSource)[V], GLchar const* (&fSource)[F])
		{
			vertex_shader = Compile(GL_VERTEX_SHADER, vSource);
			geometry_shader = GL_NONE;
			fragment_shader = Compile(GL_FRAGMENT_SHADER, fSource);
			program = glCreateProgram();

			glAttachShader(program, vertex_shader);
			glAttachShader(program, fragment_shader);
			glLinkProgram(program);

			GLint linked;
			glGetProgramiv(program, GL_LINK_STATUS, &linked);
			if(!linked)
			{
				GLint length;
				glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
				string log(length, ' ');
				glGetProgramInfoLog(program, length, &length, &log[0]);
				cout << log << endl;
				throw logic_error(log);	
			}

			cout << program << endl;
			if(glIsProgram(program))
				cout << "Program." << endl;
			else if(glIsShader(program))
				cout << "Shader." << endl;
		}
		template <int V, int G, int F>
		Effect(GLchar const* (&vSource)[V], GLchar const* (&gSource)[G], GLchar const* (&fSource)[F])
		{
			vertex_shader = Ccompile(GL_VERTEX_SHADER, vSource);
			geometry_shader = Compile(GL_GEOMETRY_SHADER, gSource);
			fragment_shader = Compile(GL_FRAGMENT_SHADER, fSource);
			program = glCreateProgram();

			glAttachShader(program, vertex_shader);
			glAttachShader(program geometry_shader);
			glAttachShader(program, fragment_shader);
			glLinkProgram(program);

			GLint linked;
			glGetProgramiv(program, GL_LINK_STATUS, &linked);
			if(!linked)
			{
				GLint length;
				glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
				string log(length, ' ');
				glGetProgramInfoLog(program, length, &length, &log[0]);
				cout << log << endl;
				throw logic_error(log);
			}
		}
		~Effect()
		{
			glDeleteProgram(program);
			glDeleteShader(vertex_shader);
			glDeleteShader(geometry_shader);
			glDeleteShader(fragment_shader);
		}

		operator GLuint()
		{
			return program;
		}
		void operator()()
		{
			glUseProgram(program);
		}

	private:
		template <int N>
		GLuint Compile(GLuint type, char const* (&source)[N])
		{
			GLuint shader = glCreateShader(type);
			glShaderSource(shader, N, source, NULL);
			glCompileShader(shader);
	
			GLint compiled;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
			if(!compiled)
			{
				GLint length;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
				string log(length, ' ');
				glGetShaderInfoLog(shader, length, &length, &log[0]);
				cout << log << endl;
				throw logic_error(log);
				return false;
			}
			return shader;
		}

		GLuint vertex_shader;
		GLuint geometry_shader;
		GLuint fragment_shader;
		GLuint program;
};

#endif