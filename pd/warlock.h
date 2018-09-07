#pragma once

#include "mob.h"
#include "callback.h"

class Warlock :public Mob, public Callback{
public:
	virtual CharSprite* Sprite();
	virtual std::string getClassName() { return "Warlock"; }
	static Mob* CreateWarlock();

	Warlock();

	virtual int damageRoll();
	virtual int attackSkill(Char* target) { return 25; }
	virtual int dr() { return 8; }
	void onZapComplete() {
		zap();
		next();
	}
	virtual void call() { next(); }
	virtual std::string description() {
		return
			std::string("When dwarves' interests have shifted from engineering to arcane arts, ") +
			std::string("warlocks have come to power in the city. They started with elemental magic, ") +
			std::string("but soon switched to demonology and necromancy.");
	}
protected:
	virtual bool canAttack(Char* enemy);
	bool doAttack(Char* enemy);

private:
	static const float TIME_TO_ZAP;

	static const std::string TXT_SHADOWBOLT_KILLED;

	void zap();
};