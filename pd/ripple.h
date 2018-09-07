#pragma once

#include "image.h"

class Ripple :public Image{
private:
	static const float TIME_TO_FADE;

	float time;
public:
	Ripple();

	void reset(int p);
	virtual void update();
};