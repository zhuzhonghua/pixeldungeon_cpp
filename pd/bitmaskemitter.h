#pragma once

#include "emitter.h"
#include "smarttexture.h"
#include "image.h"

class BitmaskEmitter :public Emitter{
private:
	SmartTexture* map;
	int mapW;
	int mapH;

public:
	BitmaskEmitter(Image* target);

protected:
	virtual void emit(int index);
};