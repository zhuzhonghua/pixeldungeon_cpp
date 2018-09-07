#pragma once

#include "mob.h"

class Spinner :public Mob{
private:
	class Fleeing:public Mob::Fleeing{
	protected:
		virtual void nowhereToRun();
	};
public:
	virtual CharSprite* Sprite();
	virtual std::string getClassName() { return "Spinner"; }
	static Mob* CreateSpinner();

	Spinner();

	virtual int damageRoll();
	virtual int attackSkill(Char* target) { return 20; }
	virtual int dr() { return 6; }
	virtual int attackProc(Char* enemy, int damage);
	virtual void move(int step);
	virtual std::string description() {
		return
			std::string("These greenish furry cave spiders try to avoid direct combat, preferring to wait in the distance ") +
			std::string("while their victim, entangled in the spinner's excreted cobweb, slowly dies from their poisonous bite.");
	}
protected:
	virtual bool act();
};