#pragma once

#include "component.h"
#include "colorblock.h"

class HealthBar :public Component{
private:
	static const int COLOR_BG = 0xFFCC0000;
	static const int COLOR_LVL = 0xFF00EE00;

	static const int HEIGHT = 2;

	ColorBlock* hpBg;
	ColorBlock* hpLvl;

	float level;

protected:
	virtual void createChildren();
	virtual void layout();
public:
	HealthBar();

	void Level(float value);
};