#pragma once

#include "mob.h"
#include "typedefine.h"

class Wraith :public Mob{
private:
	static const float SPAWN_DELAY;
	static const String LEVEL;

	int level;
public:
	Wraith();

	virtual std::string getClassName() { return "Wraith"; }
	virtual CharSprite* Sprite();
	static Mob* CreateWraith();

	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);

	virtual int damageRoll();
	virtual int attackSkill(Char* target) { return 10 + level; }
	void adjustStats(int level);
	virtual String defenseVerb() { return "evaded"; }

	virtual boolean reset() { state = WANDERING; return true; }

	virtual String description() {
		return
			std::string("A wraith is a vengeful spirit of a sinner, whose grave or tomb was disturbed. ") +
			std::string("Being an ethereal entity, it is very hard to hit with a regular weapon.");
	}
	static void spawnAround(int pos);
	static Wraith* spawnAt(int pos);
};