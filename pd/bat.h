#pragma once

#include "mob.h"

class Bat :public Mob{
public:
	virtual CharSprite* Sprite();
	virtual std::string getClassName() { return "Bat"; }
	static Mob* CreateBat();

	Bat();

	virtual int damageRoll();
	virtual int attackSkill(Char* target) { return 16; }
	virtual int dr() { return 4; }
	virtual std::string defenseVerb() { return "evaded"; }
	virtual int attackProc(Char* enemy, int damage);
	virtual std::string description();
};