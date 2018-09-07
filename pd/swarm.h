#pragma once

#include "mob.h"

class Swarm :public Mob{
public:
	int generation;

	Swarm();

	static Mob* CreateSwarm();
	virtual CharSprite* Sprite();
	virtual std::string getClassName() { return "Swarm"; }
	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);
	virtual int damageRoll();
	virtual int defenseProc(Char* enemy, int damage);
	virtual int attackSkill(Char* target) { return 12; }
	virtual std::string defenseVerb() { return "evaded"; }
	virtual std::string description();
private:
	static const float SPLIT_DELAY;
	static const std::string GENERATION;

	Swarm* split();
};