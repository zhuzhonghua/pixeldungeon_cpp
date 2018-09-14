#include "stdafx.h"

#include "program.h"

Program::Program()
{
	_handle = glCreateProgram();
}

Program::~Program()
{
	//glDeleteProgram(_handle);
}

void Program::attach(Shader& s)
{
	if (_vert._handle == 0)
	{
		_vert = s;
	}
	else if (_frag._handle == 0)
	{
		_frag = s;
	}
	else
	{
		fatalError("too much shaders");
	}

	glAttachShader(_handle, s._handle);
}

void Program::link()
{
	glLinkProgram(_handle);

	GLint success = 0;
	glGetProgramiv(_handle, GL_LINK_STATUS, (int*)&success);
	if (success == GL_FALSE)
	{
		GLint maxLen = 0;
		glGetProgramiv(_handle, GL_INFO_LOG_LENGTH, &maxLen);

		std::vector<char> infoLog(maxLen);
		glGetProgramInfoLog(_handle, maxLen, &maxLen, &infoLog[0]);

		glDeleteProgram(_handle);

		std::printf("%s\n", &infoLog[0]);
		fatalError("link error");
	}

	//glDetachShader(_handle, _vert._handle);
	//glDetachShader(_handle, _frag._handle);
	//
	//_vert.destroy();
	//_frag.destroy();
}

Attribute Program::attribute(const std::string& name)
{
	GLuint loc = glGetAttribLocation(_handle, name.c_str());
	return Attribute(loc);
}

Uniform Program::uniform(const std::string& name)
{
	GLuint loc = glGetUniformLocation(_handle, name.c_str());
	return Uniform(loc);
}

void Program::use()
{
	glUseProgram(_handle);
}

void Program::unuse()
{
	glUseProgram(0);
}

void Program::destroy()
{
	glDeleteProgram(_handle);
}