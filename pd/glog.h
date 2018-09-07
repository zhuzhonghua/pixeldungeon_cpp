#pragma once

#include "signal.h"

#include <string>

class GLog{
public:
	static const std::string TAG;

	static const std::string POSITIVE;
	static const std::string NEGATIVE;
	static const std::string WARNING;
	static const std::string HIGHLIGHT;

	static Signal<std::string> update;

	static void i(const std::string& text, ...);
	static void p(const std::string& text, ...);
	static void n(const std::string& text, ...);
	static void w(const std::string& text, ...);
	static void h(const std::string& text, ...);
private:
	static void _i(const std::string& text, va_list args);
};