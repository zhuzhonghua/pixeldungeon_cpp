#pragma once

#include "mob.h"

class Item;

class Thief :public Mob{
private:
	class Fleeing :public Mob::Fleeing{
	protected:
		virtual void nowhereToRun();
	};
public:
	Item* item;

	Thief();

	virtual CharSprite* Sprite();
	virtual std::string getClassName() { return "Thief"; }
	static Mob* CreateThief();

	virtual int damageRoll();
	virtual int attackSkill(Char* target) { return 12; }
	virtual int dr() { return 3; }
	virtual std::string description();
protected:
	static const std::string TXT_STOLE;
	static const std::string TXT_CARRIES;

	virtual float attackDelay() { return 0.5f; }

private:
	static const std::string ITEM;
};