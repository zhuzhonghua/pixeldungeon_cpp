#include "stdafx.h"

#include "image.h"
#include "noosascript.h"
#include "texturecache.h"
#include "util.h"

Image::Image() :Visual(0,0,0,0)
{
	_vertices.resize(16);
	flipVertical = false;
	flipHorizontal = false;
}

Image::Image(const Image& other) : Image()
{
	copy(other);
}

Image::Image(const Image* other) : Image()
{
	copy(*other);
	delete other;
}

Image::Image(SmartTexture* other) : Image()
{
	tex = other;
	frame(RectF(0, 0, 1, 1));
}
// TODO: coordinate
Image::Image(const std::string& tx, int left, int top, int width, int height) : Image(tx)
{
	frame(tex->uvRect(left,top,left+width,top+height));
}

Image::Image(const std::string& tx) : Image()
{
	texture(tx);
}

void Image::texture(const std::string& tx)
{
	tex = TextureCache::getTexture(tx);
	frame(RectF(0, 0, 1, 1));
}

void Image::texture(SmartTexture* tx)
{
	tex = tx;
	frame(RectF(0, 0, 1, 1));
}

void Image::copy(const Image& other)
{
	// TODO: depth copy problem
	tex = other.tex;

	_frame = other._frame;
	
	width = other.width;
	height = other.height;

	updateFrame();
	updateVertices();
}

void Image::frame(const RectF& frame)
{
	this->_frame = frame;
	
	width = GameMath::RECTFWidth(frame) * tex->width;
	height = GameMath::RECTFHeight(frame) * tex->height;
	
	updateFrame();
	updateVertices();
}

void Image::frame(int left, int top, int width, int height)
{
	frame(tex->uvRect(left, top, left + width, top + height));
}

void Image::draw()
{
	Visual::draw();
	
	NoosaScript* script = NoosaScript::get();
	
	tex->bind();
	script->camera(camera());
	
	script->uModel.valueM4(_mat);
	script->lighting(
		rm,gm,bm,am,
		ra,ga,ba,aa);
	
	// TODO: check dirty, optimization
	script->drawQuad(&_vertices[0]);
	//float vertices[] = { 0, 0, _vertices[2], _vertices[3],
	//					 1, 0, _vertices[6], _vertices[7],
	//					 1, 1, _vertices[10], _vertices[11],
	//					 0, 1, _vertices[14], _vertices[15] };
	//script->drawQuad(vertices);
}

void Image::updateVertices() {

	_vertices[0] = 0;
	_vertices[1] = 0;

	_vertices[4] = width;
	_vertices[5] = 0;

	_vertices[8] = width;
	_vertices[9] = height;

	_vertices[12] = 0;
	_vertices[13] = height;

	_dirty = true;
}

void Image::updateFrame()
{
	float left = GameMath::RECTFLeft(_frame);
	float right = GameMath::RECTFRight(_frame);
	float top = GameMath::RECTFTop(_frame);
	float bottom = GameMath::RECTFBottom(_frame);

	if (flipHorizontal) {
		_vertices[2] = right;
		_vertices[6] = left;
		_vertices[10] = left;
		_vertices[14] = right;
	}
	else {
		_vertices[2] = left;
		_vertices[6] = right;
		_vertices[10] = right;
		_vertices[14] = left;
	}

	if (flipVertical) {
		_vertices[3] = bottom;
		_vertices[7] = bottom;
		_vertices[11] = top;
		_vertices[15] = top;
	}
	else {
		_vertices[3] = top;
		_vertices[7] = top;
		_vertices[11] = bottom;
		_vertices[15] = bottom;
	}

	_dirty = true;
}