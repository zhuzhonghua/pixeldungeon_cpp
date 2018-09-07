#include "texturefilm.h"
#include "texturecache.h"
#include "util.h"

const RectF TextureFilm::FULL(0, 0, 1, 1);

TextureFilm::TextureFilm(const std::string& tx)
{
	SmartTexture* texture = TextureCache::getTexture(tx);

	texWidth = texture->width;
	texHeight = texture->height;

	add(-1, FULL);
}

TextureFilm::TextureFilm(SmartTexture* texture, int width) 
:TextureFilm(texture, width, texture->height)
{

}

TextureFilm::TextureFilm(const std::string& tx, int width, int height)
: TextureFilm::TextureFilm(TextureCache::getTexture(tx), width, height)
{
}

TextureFilm::TextureFilm(SmartTexture* tx, int width, int height)
{
	texWidth = tx->width;
	texHeight = tx->height;

	float uw = (float)width / texWidth;
	float vh = (float)height / texHeight;
	int cols = texWidth / width;
	int rows = texHeight / height;

	for (int i = 0; i < rows; i++) 
	{
		for (int j = 0; j < cols; j++) 
		{
			RectF rect(j * uw, i * vh, (j + 1) * uw, (i + 1) * vh);
			add(i * cols + j, rect);
		}
	}
}

TextureFilm::TextureFilm(TextureFilm* atlas, int key, int wid, int hei)
{
	texWidth = atlas->texWidth;
	texHeight = atlas->texHeight;

	RectF patch = atlas->get(key);

	float uw = (float)wid / texWidth;
	float vh = (float)hei / texHeight;
	int cols = (int)(width(patch) / wid);
	int rows = (int)(height(patch) / hei);

	for (int i = 0; i < rows; i++) 
	{
		for (int j = 0; j < cols; j++) 
		{
			RectF rect(j * uw, i * vh, (j + 1) * uw, (i + 1) * vh);
			GameMath::RECTFOffset(rect, GameMath::RECTFLeft(patch), GameMath::RECTFTop(patch));
			//rect.offset(GameMath::RECTFLeft(patch), GameMath::RECTFTop(patch));
			add(i * cols + j, rect);
		}
	}
}

void TextureFilm::add(int id, const RectF& rect)
{
	frames.insert(std::make_pair(id, rect));
}

RectF TextureFilm::get(int id)
{
	std::map<int, RectF>::iterator itr = frames.find(id);
	if (itr != frames.end()) return itr->second;

	return RectF();
}

float TextureFilm::width(const RectF& frame)
{
	return GameMath::RECTFWidth(frame) * texWidth;
}

float TextureFilm::height(const RectF& frame)
{
	return GameMath::RECTFHeight(frame) * texHeight;
}