#pragma once

#include "itemsprite.h"
#include "tweener.h"

class Callback;

class MissileSprite :public ItemSprite, public Tweener::Listener{
private:
	static const float SPEED;

	Callback* callback;

public:
	MissileSprite();

	void reset(int from, int to, Item* item, Callback* listener);
	void reset(int from, int to, int image, Glowing* glowing, Callback* listener);

	virtual void onComplete(Tweener* tweener);
};