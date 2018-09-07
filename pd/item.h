#pragma once

#include "bundlable.h"
//#include "hero.h"
#include "itemsprite.h"
#include "cellselector.h"
#include <string>
#include <vector>
#include "typedefine.h"

class Bag;
class Hero;
class Char;

class Item :public Bundlable, public Object{
private:
	static const char* TXT_PACK_FULL;

	static const char* TXT_BROKEN;
	static const char* TXT_GONNA_BREAK;

	static const char* TXT_TO_STRING;
	static const char* TXT_TO_STRING_X;
	static const char* TXT_TO_STRING_LVL;
	static const char* TXT_TO_STRING_LVL_X;

	static const float DURABILITY_WARNING_LEVEL;

protected:
	static const float TIME_TO_THROW;
	static const float TIME_TO_PICK_UP;
	static const float TIME_TO_DROP;

	static const std::string AC_DROP;
	static const std::string AC_THROW;

	static CellSelector::Listener* thrower;

	virtual void onThrow(int cell);
	virtual void onDetach() {
	}
public:
	static Hero* curUser;
	static Item* curItem;

	std::string defaultAction;

	std::string name = "smth";
	int image = 0;

	CLASSNAME(Item);
	CLASSOBJECT(Item);

public:
	bool stackable = false;
	bool levelKnown = false;
	bool cursed;
	bool cursedKnown;

	bool unique = false;

	int quantity;

private:
	int level = 0;
	int durability;// = maxDurability();

public:
	Item();
	virtual void actions(Hero* hero, std::vector<std::string>& actions);
	virtual bool doPickUp(Hero* hero);
	virtual void doDrop(Hero* hero);
	virtual void doThrow(Hero* hero);

	static void evoke(Hero* hero);
	virtual std::string toString();
	virtual std::string Name();
	std::string trueName() { return name; }
	int Image();
	Item* detach(Bag* container);
	Item* detachAll(Bag* container);

	virtual ItemSprite::Glowing* glowing() { return NULL; }

	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);
	virtual bool isUpgradable() { return true; }
	virtual bool isIdentified() { return levelKnown && cursedKnown; }
	virtual bool isEquipped(Hero* hero) { return false; }
	virtual int effectiveLevel();

	virtual std::string info() { return desc(); }
	virtual std::string desc() { return ""; }
	virtual int price() { return 0; }
	int Quantity() {
		return quantity;
	}

	void Quantity(int value) {
		quantity = value;
	}

	virtual void execute(Hero* hero, std::string action);
	virtual void cast(Hero* user, int dst);
	virtual void getBroken() { }
	bool isBroken() {
		return durability <= 0;
	}
	virtual void fix();
	virtual int maxDurability(int lvl) { return 1; }
			int maxDurability() { return maxDurability(level); }

	int visiblyUpgraded() {
		return levelKnown ? level : 0;
	}
	virtual Item* identify();
	virtual Item* random() { return this; }
	virtual void Level(int value) { level = value; }
	int Level() { return level; }
	virtual boolean collect(Bag* container);
	void use();

	void updateQuickslot();
	virtual Item* upgrade() {

		cursed = false;
		cursedKnown = true;

		level++;
		fix();

		return this;
	}
	virtual Item* degrade() {
		this->level--;
		fix();

		return this;
	}
	Item* degrade(int n){
		for (int i = 0; i < n; i++) {
			degrade();
		}

		return this;
	}
	int considerState(int price);
	void polish() {
		if (durability < maxDurability()) {
			durability++;
		}
	}
	std::string status();

	static Item* Virtual(const std::string& cl);
};

FACTORY(Item);