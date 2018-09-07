#pragma once

#include "image.h"

class Halo :public Image{
private:
	static const std::string CACHE_KEY;

protected:
	static const int RADIUS = 64;

	float radius;
	float brightness;

public:
	Halo();
	Halo(float radius, int color, float brightness);
	Halo* point(float x, float y);
	void Radius(float value);
};