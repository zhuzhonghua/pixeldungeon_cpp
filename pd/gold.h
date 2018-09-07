#pragma once

#include "item.h"

class Gold :public Item{
private:
	static const std::string TXT_COLLECT;
	static const std::string TXT_INFO;
	static const std::string TXT_INFO_1;
	static const std::string TXT_VALUE;

	static const std::string VALUE;
public:
	Gold();
	Gold(int value);

	virtual void actions(Hero* hero, std::vector<std::string>& actions);
	virtual bool doPickUp(Hero* hero);
	virtual bool isUpgradable() { return false; }
	virtual bool isIdentified() { return true; }

	virtual std::string info();
	virtual Item* random();

	virtual std::string getClassName() { return "Gold"; }
	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);

	static Item* Create(){ return new Gold(); }
};