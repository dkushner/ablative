#ifndef EFFECT_H
#define EFFECT_H

#include <GL/glew.h>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <map>


class Effect
{
	public:
		Effect();
		Effect(const Effect&);
		Effect(const char*, const char*);
		Effect(const char*, const char*, const char*);
		~Effect();

		operator GLuint()
		{
			return program;
		}
		void operator()()
		{
			glUseProgram(program);
		}

	private:
		static std::map<std::string, GLuint> shader_map;

		std::map<std::string, GLuint> uniform_map;
		std::map<std::string, GLuint> attribute_map;

		GLuint Compile(GLuint type, const char* source);

		GLuint attribute_index;
		GLuint vertex_shader;
		GLuint geometry_shader;
		GLuint fragment_shader;
		GLuint program;
};

#endif