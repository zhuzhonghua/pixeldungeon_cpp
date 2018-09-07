#pragma once

#include "mob.h"

class Monk :public Mob{
public:
	static const std::string TXT_DISARM;

	virtual CharSprite* Sprite();
	virtual std::string getClassName() { return "Monk"; }
	static Mob* CreateMonk();

	Monk();

	virtual int damageRoll();
	virtual int attackSkill(Char* target) { return 30; }
	virtual int dr() { return 2; }

	virtual std::string defenseVerb() { return "parried"; }
	virtual void die(const std::string& cause);
	virtual int attackProc(Char* enemy, int damage);
	virtual std::string description() {
		return
			std::string("These monks are fanatics, who devoted themselves to protecting their city's secrets from all aliens. ") +
			std::string("They don't use any armor or weapons, relying solely on the art of hand-to-hand combat.");
	}
protected:
	virtual float attackDelay() { return 0.5f; }
};