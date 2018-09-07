#pragma once

#include "mob.h"

class Tengu :public Mob{
public:

	virtual CharSprite* Sprite();
	virtual std::string getClassName() { return "Tengu"; }
	static Mob* CreateTengu();

	Tengu();
	virtual int damageRoll();
	virtual int attackSkill(Char* target) { return 20; }
	virtual int dr() { return 5; }
	virtual void die(const std::string& cause);
	virtual void notice();
	virtual std::string description();
private:
	static const int JUMP_DELAY = 5;

	int timeToJump;

	void jump();
protected:
	virtual bool getCloser(int target);
	virtual bool canAttack(Char* enemy);
	virtual bool doAttack(Char* enemy);
};