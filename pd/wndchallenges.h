#pragma once

#include "window.h"
#include "checkboxred.h"
class WndChallenges :public Window{
private:
	static const int WIDTH = 108;
	static const int TTL_HEIGHT = 12;
	static const int BTN_HEIGHT = 18;
	static const int GAP = 1;

	static const std::string TITLE;

	bool editable;
	std::vector<CheckBoxRed*> boxes;

public:
	WndChallenges(int checked, bool editable);

	virtual void onBackPressed();
};