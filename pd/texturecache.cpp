#include "stdafx.h"
#include "texturecache.h"

TextureCache* TextureCache::_inst;

TextureCache* TextureCache::inst()
{
	if (_inst == NULL)
	{
		_inst = new TextureCache();
	}

	return _inst;
}

TextureCache::TextureCache()
{

}

TextureCache::~TextureCache()
{

}

SmartTexture* TextureCache::getTexture(std::string texPath)
{
	TextureCache* tc = inst();
	std::map<std::string, SmartTexture*>::iterator mit = tc->_texMap.find(texPath);
	if (mit == tc->_texMap.end())
	{
		SmartTexture* tex = new SmartTexture(texPath);
		tc->_texMap.insert(std::make_pair(texPath, tex));
		return tex;
	}

	return mit->second;
}

SmartTexture* TextureCache::createSolid(int color)
{
	char tmp[32] = { 0 };
	sprintf(tmp, "1x1:%d", color);

	TextureCache* tc = inst();
	std::map<std::string, SmartTexture*>::iterator mit = tc->_texMap.find(tmp);
	if (mit != tc->_texMap.end())
	{
		return mit->second;
	}
	else 
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

		SDL_Surface* img = SDL_CreateRGBSurface(0, 1, 1, 32, rmask, gmask, bmask, amask);
		SDL_FillRect(img, NULL, color);

		SmartTexture* tex = new SmartTexture(img);
		tc->_texMap.insert(std::make_pair(tmp, tex));

		SDL_FreeSurface(img);

		return tex;
	}
}

SDL_Surface* TextureCache::createSurface(int w, int h, int argb)
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

	SDL_Surface* img = SDL_CreateRGBSurface(0, w, h, 32, rmask, gmask, bmask, amask);
	return img;
}

bool TextureCache::contains(const std::string& key)
{
	return inst()->_texMap.find(key) != inst()->_texMap.end();
}

void TextureCache::add(const std::string& key, SmartTexture* tx)
{
	inst()->_texMap.insert(std::make_pair(key, tx));
}