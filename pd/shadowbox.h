#pragma once

#include "ninepatch.h"
#include "define.h"
#include "util.h"

class ShadowBox :public NinePatch{
public:
	static const int SIZE = 16;

	ShadowBox() :NinePatch(Assets::SHADOW, 1) 
	{
		texture->filter(SmartTexture::LINEAR, SmartTexture::LINEAR);

		GameMath::PointFSet(&scale, SIZE, SIZE);
		//scale.set(SIZE, SIZE);
	}

	virtual void size(float width, float height) 
	{
		NinePatch::size(width / SIZE, height / SIZE);
	}

	void boxRect(float x, float y, float width, float height) 
	{
		this->x = x - SIZE;
		this->y = y - SIZE;
		size(width + SIZE * 2, height + SIZE * 2);
	}
};