#pragma once

#include "mob.h"

class Eye :public Mob{
public:
	virtual CharSprite* Sprite();
	virtual std::string getClassName() { return "Eye"; }
	static Mob* CreateEye();

	Eye();

	virtual int dr() { return 10; }
	virtual int attackSkill(Char* target) { return 30; }
	virtual bool attack(Char* enemy);
	virtual std::string description() {
		return
			std::string("One of this demon's other names is \"orb of hatred\", because when it sees an enemy, ") +
			std::string("it uses its deathgaze recklessly, often ignoring its allies and wounding them.");
	}
protected:
	virtual bool canAttack(Char* enemy);
	virtual float attackDelay() { return 1.6f; }
	virtual bool doAttack(Char* enemy);
private:
	int hitCell;
	static const std::string TXT_DEATHGAZE_KILLED;
};