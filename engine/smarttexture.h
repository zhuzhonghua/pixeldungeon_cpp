#pragma once

#include "texture.h"

class SmartTexture :public Texture{
public:
	std::string bitmaps;

	int fModeMin;
	int fModeMax;

	int wModeH;
	int wModeV;

	SmartTexture(SDL_Surface* img);
	SmartTexture(const std::string& bitmap);
	SmartTexture(const std::string& bitmap, int filtering, int wrapping);

	void filter(int minMode, int maxMode);
	void wrap(int s, int t);
	RectF uvRect(int left, int top, int right, int bottom);

	virtual void bitmap(const std::string& bitmap);
};

class Gradient :public SmartTexture{
public:
	Gradient(std::vector<int> colors);
};