#pragma once

#include "actor.h"
#include "visual.h"
#include "typedefine.h"
#include "charsprite.h"

class Pushing :public Actor{
public:
	class Effect :public Visual{
	private:
		static const float DELAY;

		Point end;

		float delay;

		Pushing* p;
	public:
		Effect(Pushing* pi);
		virtual void update();

	};
private:
	CharSprite* sprite;
	int from;
	int to;

	Effect* effect;

protected:
	virtual boolean act();
public:
	CLASSNAME(Pushing);
	Pushing(Char* ch, int from, int to);
};