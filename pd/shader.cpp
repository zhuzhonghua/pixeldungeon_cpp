#include "stdafx.h"

#include "shader.h"

void Shader::compile()
{
	glCompileShader(_handle);

	GLint success = 0;
	glGetShaderiv(_handle, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint maxLen = 0;
		glGetShaderiv(_handle, GL_INFO_LOG_LENGTH, &maxLen);

		std::vector<char> errLog(maxLen);
		glGetShaderInfoLog(_handle, maxLen, &maxLen, &errLog[0]);

		glDeleteShader(_handle);

		std::printf("%s\n", &errLog[0]);
		fatalError("compile shader fail");
	}
}

Shader Shader::createCompiled(int type, const std::string& src)
{
	Shader shader(type);
	shader.source(src);
	shader.compile();
	return shader;
}