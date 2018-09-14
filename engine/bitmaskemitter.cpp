#include "stdafx.h"
#include "bitmaskemitter.h"
#include "util.h"

BitmaskEmitter::BitmaskEmitter(Image* target)
{
	this->_target = target;

	map = target->tex;
	//mapW = map->bitmap.getWidth();
	//mapH = map->bitmap.getHeight();
	mapW = map->texSrc->w;
	mapH = map->texSrc->h;
}

void BitmaskEmitter::emit(int index)
{
	RectF frame = ((Image*)_target)->frame();
	float ofsX = GameMath::RECTFLeft(frame) * mapW;
	float ofsY = GameMath::RECTFTop(frame) * mapH;

	float x, y;
	do 
	{
		x = Random::Float(0, GameMath::RECTFWidth(frame)) * mapW;
		y = Random::Float(0, GameMath::RECTFHeight(frame)) * mapH;
	} 
	while ((map->getPixel((int)(x + ofsX), (int)(y + ofsY)) & 0x000000FF) == 0);

	factory->emit(this, index,
		_target->x + x * _target->scale.x,
		_target->y + y * _target->scale.y);
}