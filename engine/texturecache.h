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

	template<class T>
	static void add(SmartTexture* tx)
	{
		inst()->_texMap.insert(std::make_pair(typeid(T).name(), tx));
	}

	static void add(const std::string& key, SmartTexture* tx)
	{
		inst()->_texMap.insert(std::make_pair(key, tx));
	}
private:
	std::map<std::string, SmartTexture*> _texMap;
};
