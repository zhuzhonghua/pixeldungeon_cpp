#pragma once

#include "item.h"
#include "typedefine.h"
#include "itemsprite.h"

class Dewdrop :public Item{
private:
	static const String TXT_VALUE;
public:
	Dewdrop();

	virtual std::string getClassName() { return "Dewdrop"; }

	virtual boolean doPickUp(Hero* hero);

	virtual String info() {
		return "A crystal clear dewdrop.";
	}
};

class DewVial :public Item{
private:
	static const int MAX_VOLUME = 10;

	static const String AC_DRINK;

	static const float TIME_TO_DRINK;

	static const String TXT_VALUE;
	static const String TXT_STATUS;

	static const String TXT_AUTO_DRINK;
	static const String TXT_COLLECTED;
	static const String TXT_FULL;
	static const String TXT_EMPTY;

	int volume;

	static const String VOLUME;

	static const double NUM;
	static const double POW;

	static ItemSprite::Glowing* WHITE;
public:
	DewVial();

	virtual std::string getClassName() { return "DewVial"; }
	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);

	virtual void actions(Hero* hero, std::vector<std::string>& actions);
	virtual void execute(Hero* hero, std::string action);

	virtual boolean isUpgradable() {
		return false;
	}

	virtual boolean isIdentified() {
		return true;
	}
	boolean isFull() {
		return volume >= MAX_VOLUME;
	}
	void collectDew(Dewdrop* dew);
	void fill() {
		volume = MAX_VOLUME;
		updateQuickslot();
	}
	static void autoDrink(Hero* hero);

	virtual ItemSprite::Glowing* glowing() {
		return isFull() ? WHITE : NULL;
	}
	virtual String status();
	virtual String info() {
		return
			std::string("You can store excess dew in this tiny vessel for drinking it later. ") +
			std::string("If the vial is full, in a moment of deadly peril the dew will be ") +
			std::string("consumed automatically.");
	}
	//virtual String toString() {
	//	return super.toString() + " (" + status() + ")";
	//}
};