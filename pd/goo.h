#pragma once

#include "mob.h"

class Goo :public Mob{
private:
	static const float PUMP_UP_DELAY;
	bool pumpedUp;
	bool jumped;
public:

	virtual CharSprite* Sprite();
	virtual std::string getClassName() { return "Goo"; }
	static Mob* CreateGoo();

	Goo();

	virtual int damageRoll();
	virtual int attackSkill(Char* target){ return pumpedUp && !jumped ? 30 : 15; }
	virtual int dr() { return 2; }
	virtual bool act();
	virtual int attackProc(Char* enemy, int damage);
	virtual bool attack(Char* enemy){ bool result = Mob::attack(enemy); pumpedUp = false; return result; }
	
	virtual void move(int step);
	virtual void die(const std::string& cause);
	virtual void notice();
	virtual std::string description();
protected:
	virtual bool canAttack(Char* enemy){ return pumpedUp ? distance(enemy) <= 2 : Mob::canAttack(enemy); }
	virtual bool doAttack(Char* enemy);
	virtual bool getCloser(int target){ pumpedUp = false; return Mob::getCloser(target); }
};