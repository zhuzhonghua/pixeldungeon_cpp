#pragma once

#include "visual.h"

#include "smarttexture.h"
#include <string>

class Image :public Visual{
public:
	Image();
	Image(const Image* other);
	Image(const Image& other);
	Image(SmartTexture* other);
	Image(const std::string& tx);
	Image(const std::string& tx, int left, int top, int width, int height);

	void texture(SmartTexture* tx);
	void texture(const std::string& tx);
	void copy(const Image& other);
	RectF frame() { return _frame; }
	virtual void frame(const RectF& frame);
	void frame(int left, int top, int width, int height);
	void draw();

	SmartTexture* tex;
	bool flipHorizontal;
	bool flipVertical;
protected:
	void updateVertices();
	virtual void updateFrame();

	bool _dirty;
	RectF _frame;

	std::vector<float> _vertices;
};