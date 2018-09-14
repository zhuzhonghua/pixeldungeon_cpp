#pragma once

#include "GL/glew.h"
#include "shader.h"
#include "define.h"

class Attribute{
public:
	Attribute(){ _location = 0; }
	Attribute(GLuint lo) { _location = lo; }
	GLuint handle() { return _location; }
	void enable() { glEnableVertexAttribArray(_location); }
	void disable() { glDisableVertexAttribArray(_location); }

	void vertexPointer(int size, int stride, float* ptr) { glVertexAttribPointer(_location, size, GL_FLOAT, GL_FALSE, stride, (void*)ptr); }
private:
	GLuint _location;
};

class Uniform{
public:
	Uniform(){ _location = 0; }
	Uniform(GLuint lo) { _location = lo; }
	GLuint handle() { return _location; }

	void enable() { glEnableVertexAttribArray(_location); }
	void disable() { glDisableVertexAttribArray(_location); }

	void value(int v) { glUniform1i(_location, v); }
	void value1f(float v) { glUniform1f(_location, v); }
	void value2f(float v1, float v2) { glUniform2f(_location, v1, v2); }
	void value4f(float v1, float v2, float v3, float v4) { glUniform4f(_location, v1, v2, v3, v4); }
	void valueM4(const Matrix& m) { glUniformMatrix4fv(_location, 1, GL_FALSE, &(m[0][0])); }
private:
	GLuint _location;
};

class Program{
public:
	Program();
	virtual ~Program();
	GLuint handle() { return _handle; }

	void attach(Shader& s);
	void link();
	Attribute attribute(const std::string& name);
	Uniform uniform(const std::string& name);
	virtual void use();
	virtual void unuse();
	void destroy();
private:
	GLuint _handle;
	Shader _vert;
	Shader _frag;
};