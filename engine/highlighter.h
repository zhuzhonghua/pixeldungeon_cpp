#pragma once

#include <string>
#include <vector>
#include "regex/pcrecpp.h"

class Highlighter{
private:
	static pcrecpp::RE HIGHLIGHTER;
	static pcrecpp::RE STRIPPER;
public:
	std::string text;
	std::vector<bool> mask;

	Highlighter(const std::string& text);
	void inverted(std::vector<bool> &out);
	bool isHighlighted();
};