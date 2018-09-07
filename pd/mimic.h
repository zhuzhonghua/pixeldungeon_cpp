#pragma once

#include "mob.h"
#include "typedefine.h"

class Item;

class Mimic :public Mob{
private:
	int level;
	static const String LEVEL;
	static const String ITEMS;
public:
	std::vector<Item*> items;

	Mimic();

	virtual std::string getClassName() { return "Mimic"; }
	virtual CharSprite* Sprite();
	static Mob* CreateMimic();
	
	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);

	virtual int damageRoll();
	virtual int attackSkill(Char* target) { return 9 + level; }
	virtual int attackProc(Char* enemy, int damage);

	void adjustStats(int level);

	virtual void die(const std::string& src);
	virtual boolean reset() {
		state = WANDERING;
		return true;
	}
	virtual String description() {
		return
			std::string("Mimics are magical creatures which can take any shape they wish. In dungeons they almost always ") +
			std::string("choose a shape of a treasure chest, because they know how to beckon an adventurer.");
	}
	static Mimic* spawnAt(int pos, std::vector<Item*>& items);
};