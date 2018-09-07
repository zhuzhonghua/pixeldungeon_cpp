#pragma once

#include "window.h"
#include <string>

class WndOptions :public Window{
private:
	static const int WIDTH = 120;
	static const int MARGIN = 2;
	static const int BUTTON_HEIGHT = 20;

public:
	WndOptions(const std::string& title, const std::string& message, const std::vector<std::string>& options);

public:
	virtual void onSelect(int index) {};
};