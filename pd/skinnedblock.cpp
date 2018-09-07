#include "skinnedblock.h"

SkinnedBlock::SkinnedBlock(float width, float height, const std::string& tx)
:autoAdjust(false)
{
	init(width, height, tx);
}

void SkinnedBlock::init(float width, float height, const std::string& tx)
{
	texture(tx);
	tex->wrap(Texture::REPEAT, Texture::REPEAT);
	size(width, height);
}

void SkinnedBlock::frame(const RectF& frame)
{
	_scaleX = 1;
	_scaleY = 1;

	_offsetX = 0;
	_offsetY = 0;
	
	Image::frame(RectF(0, 0, 1, 1));
}

void SkinnedBlock::updateFrame()
{
	if (autoAdjust) 
	{
		while (_offsetX > tex->width) 
		{
			_offsetX -= tex->width;
		}
		while (_offsetX < -tex->width) 
		{
			_offsetX += tex->width;
		}
		while (_offsetY > tex->height)
		{
			_offsetY -= tex->height;
		}
		while (_offsetY < -tex->height)
		{
			_offsetY += tex->height;
		}
	}
	
	float tw = 1.0f / tex->width;
	float th = 1.0f / tex->height;
	
	float u0 = _offsetX * tw;
	float v0 = _offsetY * th;
	float u1 = u0 + width * tw / _scaleX;
	float v1 = v0 + height * th / _scaleY;
	
	_vertices[2] = u0;
	_vertices[3] = v0;
	
	_vertices[6] = u1;
	_vertices[7] = v0;
	
	_vertices[10] = u1;
	_vertices[11] = v1;
	
	_vertices[14] = u0;
	_vertices[15] = v1;

	_dirty = true;
}

void SkinnedBlock::offsetTo(float x, float y)
{
	_offsetX = x;
	_offsetY = y;
	updateFrame();
}

void SkinnedBlock::offset(float x, float y) 
{
	_offsetX += x;
	_offsetY += y;
	updateFrame();
}