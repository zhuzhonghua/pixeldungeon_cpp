#pragma once

#include "mob.h"

class Succubus :public Mob{
public:
	virtual CharSprite* Sprite();
	virtual std::string getClassName() { return "Succubus"; }
	static Mob* CreateSuccubus();

	Succubus();

	virtual int damageRoll();
	virtual int attackProc(Char* enemy, int damage);
};