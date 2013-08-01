#include "Effect.h"

std::map<std::string, GLuint> Effect::shader_map;

Effect::Effect()
{
	this->attribute_index = -1;
	this->vertex_shader = -1;
	this->geometry_shader = -1;
	this->fragment_shader = -1;
	this->program = -1;
}
Effect::Effect(const Effect& effect)
{
	this->attribute_index = effect.attribute_index;
	this->attribute_map = effect.attribute_map;
	this->uniform_map = effect.uniform_map;
	this->fragment_shader = effect.fragment_shader;
	this->geometry_shader = effect.geometry_shader;
	this->vertex_shader = effect.vertex_shader;
	this->program = effect.program;
}
Effect::Effect(const char* vertFile, const char* fragFile)
{
	std::ifstream in;
	std::string source;

	if(shader_map.find(vertFile) == shader_map.end())
	{
		in.open(vertFile);
		source.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
		vertex_shader = Compile(GL_VERTEX_SHADER, source.c_str());
		shader_map[vertFile] = vertex_shader;
		in.close();
	}
	else
		vertex_shader = shader_map.find(vertFile)->second;

	if(shader_map.find(fragFile) == shader_map.end())
	{
		in.open(fragFile);
		source.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
		fragment_shader = Compile(GL_FRAGMENT_SHADER, source.c_str());
		shader_map[fragFile] = fragment_shader;
		in.close();
	}
	else
		fragment_shader = shader_map.find(fragFile)->second;

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
		std::string log(length, ' ');
		glGetProgramInfoLog(program, length, &length, &log[0]);
		std::cout << log << std::endl;
		throw std::logic_error(log);	
	}
}
Effect::Effect(const char* vertFile, const char* geomFile, const char* fragFile)
{
	std::ifstream in;
	std::string source;

	if(shader_map.find(vertFile) == shader_map.end())
	{
		in.open(vertFile);
		source.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
		vertex_shader = Compile(GL_VERTEX_SHADER, source.c_str());
		shader_map[vertFile] = vertex_shader;
		in.close();
	}
	else
		vertex_shader = shader_map.find(vertFile)->second;
			
	if(shader_map.find(geomFile) == shader_map.end())
	{
		in.open(geomFile);
		source.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
		geometry_shader = Compile(GL_GEOMETRY_SHADER, source.c_str());
		shader_map[geomFile] = geometry_shader;
		in.close();
	}
	else
		geometry_shader = shader_map.find(geomFile)->second;
			
	if(shader_map.find(fragFile) == shader_map.end())
	{
		in.open(fragFile);
		source.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
		fragment_shader = Compile(GL_FRAGMENT_SHADER, source.c_str());
		shader_map[fragFile] = fragment_shader;
		in.close();
	}
	else
		fragment_shader = shader_map.find(fragFile)->second;
			
	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, geometry_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if(!linked)
	{
		GLint length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		std::string log(length, ' ');
		glGetProgramInfoLog(program, length, &length, &log[0]);
		std::cout << log << std::endl;
		throw std::logic_error(log);
	}
}
Effect::~Effect()
{
	glDeleteProgram(program);
	glDeleteShader(vertex_shader);
	glDeleteShader(geometry_shader);
	glDeleteShader(fragment_shader);
}

GLuint Effect::Compile(GLuint type, const char* source)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if(!compiled)
	{
		GLint length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		std::string log(length, ' ');
		glGetShaderInfoLog(shader, length, &length, &log[0]);
		std::cout << log << std::endl;
		throw std::logic_error(log);
		return false;
	}
	return shader;
}