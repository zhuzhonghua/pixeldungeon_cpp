#pragma once

#include "image.h"

class Char;
class Wound :public Image{
private:
	static const float TIME_TO_FADE;

	float time;

public:
	Wound();
	void reset(int p);

	virtual void update();
	static void hit(Char* ch);
	static void hit(Char* ch, float angle);
	static void hit(int pos);
	static void hit(int pos, float angle);
};