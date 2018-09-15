#pragma once

#include "define.h"
#include <map>
#include <string>

class SmartTexture;

class Atlas{
public:
	SmartTexture* tx;

	Atlas(SmartTexture* tx);

	void add(const std::string& key, int left, int top, int right, int bottom);

	void add(const std::string& key, RectF rect);

	void grid(int width);

	void grid(int width, int height);

	void grid(int left, int top, int width, int height, int cols);

	RectF get(int index);

	RectF get(const std::string& key);

	float width(RectF rect);

	float height(RectF rect);

	static RectF uvRect(SmartTexture* tx, int left, int top, int right, int bottom);
protected:
	std::map<std::string, RectF> namedFrames;
	float uvLeft;
	float uvTop;
	float uvWidth;
	float uvHeight;
	int cols;


};