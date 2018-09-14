#pragma once

#include "program.h"
#include "shader.h"

#include <map>

class Script:public Program{
public:
	void compile(const std::string& vertSrc, const std::string& fragSrc)
	{
		attach(Shader::createCompiled(Shader::VERTEX, vertSrc));
		attach(Shader::createCompiled(Shader::FRAGMENT, fragSrc));		

		link();
	}

	static Script* get(const std::string& c);

	static void reset();
	virtual void unuse() {}
protected:
	static std::map<std::string, Script*> _all;
	static Script* _curScript;
	static std::string _curScriptClass;
};