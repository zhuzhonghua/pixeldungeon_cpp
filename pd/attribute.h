#pragma once

#include "GL/glew.h"

class Attribute{
public:
	Attribute(GLuint lo) { _location = lo; }
	void enable() { glEnableVertexAttribArray(_location); }
	void disable() { glDisableVertexAttribArray(_location); }
private:
	GLuint _location;
};