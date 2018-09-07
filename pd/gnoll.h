#pragma once

#include "mob.h"

class Gnoll :public Mob{
public:
	Gnoll();

	static Mob* CreateGnoll();

	virtual CharSprite* Sprite();

	virtual int damageRoll();
	virtual int attackSkill(Char* target) { return 11; }
	virtual int dr() { return 2; }
	virtual void die(const std::string& cause);

	virtual std::string getClassName() { return "Gnoll"; }
	virtual std::string description();
};