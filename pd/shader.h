#pragma once

#include "gl/glew.h"
#include <string>
#include <vector>

class Shader{
public:
	friend class Program;

	static const int VERTEX = GL_VERTEX_SHADER;
	static const int FRAGMENT = GL_FRAGMENT_SHADER;

	Shader()
	{
		_handle = 0;
	}

	Shader(int type) 
	{
		_handle = glCreateShader(type);
	}

	void source(const std::string& src)
	{
		const char* contentsPtr = src.c_str();
		glShaderSource(_handle, 1, &contentsPtr, NULL);
	}

	void compile();

	void destroy() { glDeleteShader(_handle); _handle = 0; }

	static Shader createCompiled(int type, const std::string& src);
private:
	GLuint _handle;
};