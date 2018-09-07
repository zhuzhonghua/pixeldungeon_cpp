#pragma once

#include "mob.h"

class DM300 :public Mob{
public:
	virtual CharSprite* Sprite();
	virtual std::string getClassName() { return "DM300"; }
	static Mob* CreateDM300();

	DM300();

	virtual int damageRoll();
	virtual int attackSkill(Char* target) { return 28; }
	virtual int dr() { return 10; }
	virtual bool act();
	virtual void move(int step);
	virtual void die(const std::string& cause);
	virtual void notice();
	virtual std::string description() {
		return
			std::string("This machine was created by the Dwarves several centuries ago. Later, Dwarves started to replace machines with ") +
			std::string("golems, elementals and even demons. Eventually it led their civilization to the decline. The DM-300 and similar ") +
			std::string("machines were typically used for construction and mining, and in some cases, for city defense.");
	}
};