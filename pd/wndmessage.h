#pragma once

#include "window.h"

class WndMessage :public Window{
private:
	static const int WIDTH_P = 120;
	static const int WIDTH_L = 144;
	static const int MARGIN = 4;

public:
	WndMessage(const std::string& text);
};