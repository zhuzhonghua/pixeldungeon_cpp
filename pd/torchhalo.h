#pragma once

#include "halo.h"

class CharSprite;

class TorchHalo :public Halo{
private:
	CharSprite* target;

	float phase = 0;

public:
	TorchHalo(CharSprite* sprite);

	virtual void update();
	virtual void draw();
	void putOut() { phase = -1; }
};