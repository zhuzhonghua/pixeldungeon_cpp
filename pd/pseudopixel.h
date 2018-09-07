#pragma once

#include "image.h"
#include "texturecache.h"
#include "util.h"

class PseudoPixel :public Image{
public:
	PseudoPixel() :Image(TextureCache::createSolid(0xFFFFFFFF))
	{

	}
	PseudoPixel(float x, float y, int c) :PseudoPixel()
	{
		this->x = x;
		this->y = y;
		color(c);
	}

	void size(float w, float h)
	{
		GameMath::PointFSet(&scale, w, h);
		//scale.set(w, h);
	}
	void size(float value)
	{
		GameMath::PointFSet(&scale, value);
		//scale.set(value);
	}
};