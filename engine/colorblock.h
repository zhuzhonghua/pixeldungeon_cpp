#pragma once

#include "image.h"
#include "resizable.h"
#include "texturecache.h"
#include "util.h"

class ColorBlock :public Image, public Resizable{
public:
	ColorBlock(float width, float height, int c) :Image(TextureCache::createSolid(c))
	{
		GameMath::PointFSet(&scale, width, height);
		//scale.set(width, height);
		GameMath::PointFSet(&origin, 0, 0);
		//origin.set(0, 0);
	}

	virtual void size(float width, float height)
	{
		GameMath::PointFSet(&scale, width, height);
		//scale.set(width, height);
	}

	virtual float width()
	{
		return scale.x;
	}
	virtual float height()
	{
		return scale.y;
	}
};