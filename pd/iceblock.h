#pragma once

#include "gizmo.h"

class CharSprite;

class IceBlock :public Gizmo{
private:
	float phase;

	CharSprite* target;

public:
	IceBlock(CharSprite* target);
	virtual void update();
	void melt();
	static IceBlock* freeze(CharSprite* sprite);
};