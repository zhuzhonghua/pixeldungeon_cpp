#pragma once
#include "mob.h"

class Scorpio :public Mob{
public:
	virtual CharSprite* Sprite();
	virtual std::string getClassName(){ return "Scorpio"; }
	static Mob* CreateScorpio();

	Scorpio();

	virtual int damageRoll();
	virtual int attackSkill(Char* target) { return 36; }
	virtual int dr() { return 16; }
	virtual int attackProc(Char* enemy, int damage);
	virtual std::string description() {
		return
			std::string("These huge arachnid-like demonic creatures avoid close combat by all means, ") +
			std::string("firing crippling serrated spikes from long distances.");
	}
protected:
	virtual bool canAttack(Char* enemy);
	virtual bool getCloser(int target);
	virtual void dropLoot();
};