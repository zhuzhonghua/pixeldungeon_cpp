#pragma once

#include "tag.h"

class BitmapText;
class Image;

class DangerIndicator :public Tag{
public:
	static const int COLOR = 0xFF4C4C;

private:
	BitmapText* number;
	Image* icon;

	int enemyIndex = 0;

	int lastNumber = -1;

	void placeNumber();
public:
	DangerIndicator();
	virtual void update();
protected:
	virtual void createChildren();
	virtual void layout();
	virtual void onClick();
};