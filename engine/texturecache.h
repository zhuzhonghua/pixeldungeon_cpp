#pragma once

#include <map>
#include <string>
#include "smarttexture.h"

class TextureCache
{
	static TextureCache* _inst;
	
	static TextureCache* inst();
	TextureCache();
public:	
	~TextureCache();

	static SmartTexture* getTexture(std::string);
	static SmartTexture* createSolid(int color);

	static SDL_Surface* createSurface(int w, int h,int argb);

	static bool contains(const std::string& key);
	static void add(const std::string& key, SmartTexture* tx);
private:
	std::map<std::string, SmartTexture*> _texMap;
};
