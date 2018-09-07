#pragma once

#include "mob.h"

class Elemental :public Mob{
public:
	virtual CharSprite* Sprite();
	virtual std::string getClassName() { return "Elemental"; }
	static Mob* CreateElemental();

	Elemental();

	virtual int damageRoll();
	virtual int attackSkill(Char* target) { return 25; }
	virtual int dr() { return 5; }
	virtual int attackProc(Char* enemy, int damage);
	virtual void add(Buff* buff);
	virtual std::string description() {
		return
			std::string("Wandering fire elementals are a byproduct of summoning greater entities. ") +
			std::string("They are too chaotic in their nature to be controlled by even the most powerful demonologist.");
	}
};