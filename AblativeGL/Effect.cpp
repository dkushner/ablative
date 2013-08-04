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

	PrintActiveAttributes();
	PrintActiveUniforms();
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

GLuint Effect::GetAttributeIndex(const char* name)
{
	GLuint loc = glGetAttribLocation(program, name);
	if(loc > -1)
		attribute_map[name] = loc;
	
	return loc;
}

void Effect::SetUniform(const char* name, glm::vec3& v)
{
	GLuint loc;
	ResourceIterator itr = uniform_map.find(name);
	if(itr == uniform_map.end())
	{
		loc = glGetUniformLocation(program, name);
		uniform_map[name] = loc;
	}
	else
		loc = itr->second;
	
	glUniform3fv(loc, 1, glm::value_ptr(v));
}
void Effect::SetUniform(const char* name, glm::vec4& v)
{
	GLuint loc;
	ResourceIterator itr = uniform_map.find(name);
	if(itr == uniform_map.end())
	{
		loc = glGetUniformLocation(program, name);
		uniform_map[name] = loc;
	}
	else
		loc = itr->second;

	glUniform4fv(loc, 1, glm::value_ptr(v));
}
void Effect::SetUniform(const char* name, bool transpose, glm::mat3& m)
{
	GLuint loc;
	ResourceIterator itr = uniform_map.find(name);
	if(itr == uniform_map.end())
	{
		loc = glGetUniformLocation(program, name);
		uniform_map[name] = loc;
	}
	else
		loc = itr->second;

	glUniformMatrix3fv(loc, 1, transpose, glm::value_ptr(m));
}
void Effect::SetUniform(const char* name, bool transpose, glm::mat4& m)
{
	GLuint loc;
	ResourceIterator itr = uniform_map.find(name);
	if(itr == uniform_map.end())
	{
		loc = glGetUniformLocation(program, name);
		uniform_map[name] = loc;
	}
	else
		loc = itr->second;
	
	glUniformMatrix4fv(loc, 1, transpose, glm::value_ptr(m));
}
void Effect::SetUniform(const char* name, GLfloat f)
{
	GLuint loc;
	ResourceIterator itr = uniform_map.find(name);
	if(itr == uniform_map.end())
	{
		loc = glGetUniformLocation(program, name);
		uniform_map[name] = loc;
	}
	else
		loc = itr->second;

	glUniform1f(loc, f);
}
void Effect::SetUniform(const char* name, GLint i)
{
	GLuint loc;
	ResourceIterator itr = uniform_map.find(name);
	if(itr == uniform_map.end())
	{
		loc = glGetUniformLocation(program, name);
		uniform_map[name] = loc;
	}
	else
		loc = itr->second;

	glUniform1i(loc, i);
}

void Effect::PrintActiveAttributes()
{
	GLint attrib_count;
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &attrib_count);

	GLchar* name;
	GLint name_length;
	GLint size;
	GLenum type;

	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &name_length);
	name = (GLchar*)malloc(name_length);

	printf("\n----------ATTRIBUTES-----------\n");
	for(int atr = 0; atr < attrib_count; atr++)
	{
		glGetActiveAttrib(program, atr, name_length, NULL, &size, &type, name);
		
		printf("Name: %s\n", name);
		printf("Size: %d\n", size);
		switch(type)
		{
			case GL_FLOAT:
				printf("Type: float\n");
				break;
			case GL_FLOAT_VEC2:
				printf("Type: vec2\n");
				break;
			case GL_FLOAT_VEC3:
				printf("Type: vec3\n");
				break;
			case GL_FLOAT_VEC4:
				printf("Type: vec4\n");
				break;
			case GL_FLOAT_MAT3:
				printf("Type: mat3\n");
				break;
			case GL_FLOAT_MAT4:
				printf("Type: mat4\n");
				break;
			default:
				printf("Type: %d(unknown)\n", type);
				break;
		}
		printf("\n");
	}
}
void Effect::PrintActiveUniforms()
{
	GLint uniform_count;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &uniform_count);

	GLchar* name;
	GLint name_length;
	GLint size;
	GLenum type;

	glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &name_length);
	name = (GLchar*)malloc(name_length);

	printf("\n-----------UNIFORMS-----------\n");
	for(int uni = 0; uni < uniform_count; uni++)
	{
		glGetActiveUniform(program, uni, name_length, NULL, &size, &type, name);
		
		printf("Name: %s\n", name);
		printf("Size: %d\n", size);
		switch(type)
		{
			case GL_FLOAT:
				printf("Type: float\n");
				break;
			case GL_FLOAT_VEC2:
				printf("Type: vec2\n");
				break;
			case GL_FLOAT_VEC3:
				printf("Type: vec3\n");
				break;
			case GL_FLOAT_VEC4:
				printf("Type: vec4\n");
				break;
			case GL_FLOAT_MAT3:
				printf("Type: mat3\n");
				break;
			case GL_FLOAT_MAT4:
				printf("Type: mat4\n");
				break;
			default:
				printf("Type: %d(unknown)\n", type);
				break;
		}
		printf("\n");
	}
}