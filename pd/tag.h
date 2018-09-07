#pragma once

#include "button.h"

class NinePatch;

class Tag :public Button{
private:
	float r;
	float g;
	float b;
protected:
	NinePatch* bg;

	float lightness = 0;

	virtual void createChildren();
	virtual void layout();
public:
	Tag(int color);

	void flash();

	virtual void update();
};