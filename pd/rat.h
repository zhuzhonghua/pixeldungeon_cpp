#pragma once

#include "mob.h"

class Rat :public Mob{
public:
	Rat();

	static Mob* CreateRat();

	virtual CharSprite* Sprite();
	virtual int damageRoll();
	virtual int attackSkill(Char* target) { return 8; }
	virtual int dr() { return 1; }
	virtual void die(const std::string& cause);
	virtual std::string getClassName() { return "Rat"; }
	virtual std::string description();
};