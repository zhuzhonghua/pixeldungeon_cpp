#pragma once

#include <map>
#include "define.h"
#include "smarttexture.h"

class TextureFilm{
private:
	static const RectF FULL;

	int texWidth;
	int texHeight;

protected:
	std::map<int, RectF> frames;
public:
	TextureFilm(const std::string& tx);
	TextureFilm(SmartTexture* texture, int width);
	TextureFilm(const std::string& tx, int width, int height);
	TextureFilm(SmartTexture* tx, int width, int height);
	TextureFilm(TextureFilm* atlas, int key, int width, int height);

	void add(int id, const RectF& rect);
	RectF get(int id);
	float width(const RectF& frame);
	float height(const RectF& frame);
};