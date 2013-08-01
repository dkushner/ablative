#ifndef EFFECT_H
#define EFFECT_H

#include <GL/glew.h>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <map>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

typedef std::map<std::string, GLuint>::iterator ResourceIterator;

class Effect
{
	public:
		Effect();
		Effect(const Effect&);
		Effect(const char*, const char*);
		Effect(const char*, const char*, const char*);
		~Effect();

		GLuint GetAttributeIndex(const char*);

		void SetUniform(const char*, glm::vec3&);
		void SetUniform(const char*, glm::vec4&);
		void SetUniform(const char*, bool transpose, glm::mat3&);
		void SetUniform(const char*, bool transpose, glm::mat4&);
		void SetUniform(const char*, GLfloat);
		void SetUniform(const char*, GLint);

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
		void PrintActiveUniforms();
		void PrintActiveAttributes();

		GLuint attribute_index;
		GLuint vertex_shader;
		GLuint geometry_shader;
		GLuint fragment_shader;
		GLuint program;
};

#endif