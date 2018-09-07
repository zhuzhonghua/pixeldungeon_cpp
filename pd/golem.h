#pragma once

#include "mob.h"

class Golem :public Mob{
public:
	virtual CharSprite* Sprite();
	virtual std::string getClassName(){ return "Golem"; }
	static Mob* CreateGolem();

	Golem();

	virtual int damageRoll();

	virtual int attackSkill(Char* target) { return 28; }
	virtual int dr() { return 12; }
	virtual std::string defenseVerb() { return "blocked"; }
	virtual void die(const std::string& cause);
	virtual std::string description() {
		return
			std::string("The Dwarves tried to combine their knowledge of mechanisms with their newfound power of elemental binding. ") +
			std::string("They used spirits of earth as the \"soul\" for the mechanical bodies of golems, which were believed to be ") +
			std::string("most controllable of all. Despite this, the tiniest mistake in the ritual could cause an outbreak.");
	}
protected:
	virtual float attackDelay() { return 1.5f; }
};