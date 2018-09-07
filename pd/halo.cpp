#include "stdafx.h"
#include "halo.h"
#include "texturecache.h"
#include "util.h"

const std::string Halo::CACHE_KEY = "Halo";

Halo::Halo()
{
	radius = RADIUS;
	brightness = 1;

	if (!TextureCache::contains(CACHE_KEY)) 
	{
		Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
#endif

		SDL_Surface* img = SDL_CreateRGBSurface(0, RADIUS * 2, RADIUS * 2, 32, rmask, gmask, bmask, amask);
		Draw_Circle(img, RADIUS, RADIUS, RADIUS * 0.75f, 0xFFFFFFFF);
		Draw_Circle(img, RADIUS, RADIUS, RADIUS, 0x88FFFFFF);
		TextureCache::add(CACHE_KEY, new SmartTexture(img));
	}

	texture(CACHE_KEY);

	GameMath::PointFSet(&origin, RADIUS);
	//origin.set(RADIUS);
}

Halo::Halo(float radius, int color, float brightness)
:Halo()
{
	hardlight(color);
	alpha(this->brightness = brightness);
	Radius(radius);
}

Halo* Halo::point(float x, float y)
{
	this->x = x - RADIUS;
	this->y = y - RADIUS;
	return this;
}

void Halo::Radius(float value)
{
	GameMath::PointFSet(&scale, (this->radius = value) / RADIUS);
	//scale.set((this.radius = value) / RADIUS);
}
