#pragma once

#include "buff.h"
#include "herodoom.h"
#include "typedefine.h"

class Burning :public Buff, public HeroDoom{
public:
	CLASSNAME(Burning);

	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);

	virtual bool act();
	virtual void reignite(Char* ch);
	virtual int icon();
	static float duration(Char* ch);

	virtual void onDeath();
	virtual std::string toString() { return ""; };
private:
	static const std::string TXT_BURNS_UP;
	static const std::string TXT_BURNED_TO_DEATH;

	static const float DURATION;

	float left;

	static const std::string LEFT;
};