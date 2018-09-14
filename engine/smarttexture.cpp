#include "stdafx.h"

#include "smarttexture.h"
#include "util.h"
#include "texturecache.h"

void SmartTexture::filter(int minMode, int maxMode)
{
	Texture::filter(fModeMin = minMode, fModeMax=maxMode);
}

void SmartTexture::wrap(int s, int t)
{
	Texture::wrap(wModeH = s, wModeV = t);
}

RectF SmartTexture::uvRect(int left, int top, int right, int bottom)
{
	return GameMath::RECTF(
		(float)left / width,
		(float)top / height,
		(float)right / width,
		(float)bottom / height);
}

void SmartTexture::bitmap(const std::string& bp)
{
	Texture::bitmap(bp);

	this->bitmaps = bp;
}

SmartTexture::SmartTexture(SDL_Surface* img)
{
	Texture::bitmap(img);
	filter(GL_NEAREST, GL_NEAREST);
	wrap(GL_CLAMP, GL_CLAMP);
}

SmartTexture::SmartTexture(const std::string& bitmap) :SmartTexture(bitmap, GL_NEAREST, GL_CLAMP)
{

}

SmartTexture::SmartTexture(const std::string& bp, int filtering, int wrapping)
{
	bitmap(bp);
	filter(filtering,filtering);
	wrap(wrapping, wrapping);
}

Gradient::Gradient(std::vector<int> colors)
:SmartTexture(TextureCache::createSurface(colors.size(), 1, 0))
{
	for (int i = 0; i < colors.size(); i++) {
		setPixel(i, 0, colors[i]);
	}
	Texture::bitmap(texSrc);

	filter(LINEAR, LINEAR);
	wrap(CLAMP, CLAMP);

	TextureCache::add("Gradient", this);
}
