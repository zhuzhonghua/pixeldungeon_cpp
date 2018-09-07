#pragma once

#include "tweener.h"

class Visual;
class AlphaTweener :public Tweener{
public:
	Visual* image;

	float start;
	float delta;

	AlphaTweener(Visual* image, float alpha, float time);

protected:
	virtual void updateValues(float progress);
};