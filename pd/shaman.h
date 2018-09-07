#pragma once

#include "mob.h"
#include "callback.h"

class Shaman :public Mob, public Callback{
public:
	virtual CharSprite* Sprite();
	virtual std::string getClassName() { return "Shaman"; }
	static Mob* CreateShaman();

	virtual void call() { next(); }

	Shaman();
	virtual int damageRoll();
	virtual int attackSkill(Char* target) { return 11; }
	virtual int dr() { return 4; }
	virtual std::string description();
protected:
	virtual bool canAttack(Char* enemy);
	virtual bool doAttack(Char* enemy);
private:
	static const float TIME_TO_ZAP;
	static const std::string TXT_LIGHTNING_KILLED;
};