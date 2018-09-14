#pragma once

#include "visual.h"
#include "smarttexture.h"

class NinePatch :public Visual{
public:
	SmartTexture* texture;

	NinePatch(const std::string& tx, int margin);
	NinePatch(const std::string& tx, int left, int top, int right, int bottom);
	NinePatch(const std::string& tx, int x, int y, int w, int h, int margin);
	NinePatch(const std::string& tx, int x, int y, int w, int h, int left, int top, int right, int bottom);

	int marginLeft() { return _marginLeft; }
	int marginRight() { return _marginRight; }
	int marginTop() { return _marginTop; }
	int marginBottom() { return _marginBottom; }
	int marginHor() { return _marginLeft + _marginRight; }
	int marginVer() { return _marginTop + _marginBottom; }
	float innerWidth() { return width - _marginLeft - _marginRight; }
	float innerHeight() { return height - _marginTop - _marginBottom; }
	float innerRight() { return width - _marginRight; }
	float innerBottom() { return height - _marginBottom; }
	virtual void size(float width, float height);

	virtual void draw();
protected:
	std::vector<float> _vertices;

	RectF _outterF;
	RectF _innerF;

	int _marginLeft;
	int _marginRight;
	int _marginTop;
	int _marginBottom;

	float _nWidth;
	float _nHeight;

	void updateVertices();
};