#pragma once

#include "mob.h"

class Brute :public Mob{
public:

	virtual CharSprite* Sprite();
	virtual std::string getClassName() { return "Brute"; }
	virtual void restoreFromBundle(Bundle* bundle);
	static Mob* CreateBrute();

	Brute();

	virtual int damageRoll();
	virtual int attackSkill(Char* target) { return 20; }
	virtual int dr() { return 8; }
	virtual void damage(int dmg, const std::string& src);
	virtual std::string description();
private:
	static const std::string TXT_ENRAGED;
	bool enraged;
};