#pragma once

#include "image.h"

class Compass :public Image{
private:
	static const float RAD_2_G;
	static const float RADIUS;

	int cell;
	PointF cellCenter;

	PointF lastScroll;

public:
	Compass(int cell);

	virtual void update();
};