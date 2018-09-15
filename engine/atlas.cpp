#include "stdafx.h"

#include "atlas.h"
#include "smarttexture.h"
#include "util.h"

Atlas::Atlas(SmartTexture* tx)
{
	this->tx = tx;
	tx->atlas = this;
}

void Atlas::add(const std::string& key, int left, int top, int right, int bottom) 
{
	add(key, uvRect(tx, left, top, right, bottom));
}

void Atlas::add(const std::string& key, RectF rect) 
{
	namedFrames.insert(std::make_pair(key, rect));
}

void Atlas::grid(int width) 
{
	grid(width, tx->height);
}

void Atlas::grid(int width, int height) 
{
	grid(0, 0, width, height, tx->width / width);
}

void Atlas::grid(int left, int top, int width, int height, int cols) 
{
	uvLeft = (float)left / tx->width;
	uvTop = (float)top / tx->height;
	uvWidth = (float)width / tx->width;
	uvHeight = (float)height / tx->height;
	this->cols = cols;
}

RectF Atlas::get(int index) 
{
	float x = index % cols;
	float y = index / cols;
	float l = uvLeft + x * uvWidth;
	float t = uvTop + y * uvHeight;
	return RectF(l, t, l + uvWidth, t + uvHeight);
}

RectF Atlas::get(const std::string& key) 
{
	return namedFrames.find(key)->second;
}

float Atlas::width(RectF rect) 
{
	return GameMath::RECTFWidth(rect) * tx->width;
}

float Atlas::height(RectF rect) 
{
	return GameMath::RECTFHeight(rect) * tx->height;
}

RectF Atlas::uvRect(SmartTexture* tx, int left, int top, int right, int bottom) 
{
	return RectF(
		(float)left / tx->width,
		(float)top / tx->height,
		(float)right / tx->width,
		(float)bottom / tx->height);
}