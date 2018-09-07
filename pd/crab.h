#pragma once

#include "mob.h"

class Crab :public Mob{
public:
	Crab();

	static Mob* CreateCrab();
	virtual CharSprite* Sprite();
	virtual int damageRoll();
	virtual int attackSkill(Char* target) { return 12; }
	virtual int dr() { return 4; }
	virtual std::string defenseVerb() { return "parried"; }
	virtual void die(const std::string& cause);

	virtual std::string getClassName() { return "Crab"; }
	virtual std::string description();
};