#pragma once

#include "image.h"
#include <string>

class SkinnedBlock:public Image {
protected:
	float _scaleX;
	float _scaleY;

	float _offsetX;
	float _offsetY;

protected:
	virtual void updateFrame();
public:
	bool autoAdjust;

	SkinnedBlock(float width, float height, const std::string& tx);
	void init(float width, float height, const std::string& tx);

	virtual void frame(const RectF& frame);
	void offsetTo(float x, float y);
	void offset(float x, float y);
	float offsetX() { return _offsetX; }
	float offsetY() { return _offsetY; }

	void scale(float x, float y) 
	{ 
		_scaleX = x;
		_scaleY = y;
		updateFrame(); 
	}

	void size(float w, float h) 
	{
		this->width = w;
		this->height = h;
		updateFrame();
		updateVertices();
	}
};
