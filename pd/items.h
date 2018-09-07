#pragma once

#include "item.h"
#include "typedefine.h"
#include "bpt.h"

class Bomb :public Item{
public:
	Bomb();

	virtual std::string getClassName() { return "Bomb"; }
	static Item* Create() { return new Bomb(); }

	virtual boolean isUpgradable() {
		return false;
	}
	virtual boolean isIdentified() {
		return true;
	}
	virtual Item* random();
	virtual int price() {
		return 10 * quantity;
	}
	virtual String info() {
		return BPT::getText("lang.Bomb_Info");
			//"This is a relatively small bomb, filled with black powder. Conveniently, its fuse is lit automatically when the bomb is thrown.";
	}
protected:
	virtual void onThrow(int cell);
};

class Honeypot :public Item{
public:
	static const String AC_SHATTER;

	Honeypot();
	virtual std::string getClassName() { return "Honeypot"; }
	static Item* Create() { return new Honeypot(); }

	virtual void actions(Hero* hero, std::vector<std::string>& actions);
	virtual void execute(Hero* hero, std::string action);
	virtual boolean isUpgradable() {
		return false;
	}
	virtual boolean isIdentified() {
		return true;
	}
	virtual int price() {
		return 50 * quantity;
	}
	virtual String info() {
		return BPT::getText("lang.Honeypot_Info");
			//"There is not much honey in this small honeypot, but there is a golden bee there and it doesn't want to leave it.";
	}
private:
	void shatter(int pos);
};

class Amulet :public Item{
private:
	static const String AC_END;

	void showAmuletScene(boolean showText);
public:
	Amulet();
	CLASSNAME(Amulet);

	virtual void actions(Hero* hero, std::vector<std::string>& actions);
	virtual void execute(Hero* hero, std::string action);
	virtual bool doPickUp(Hero* hero);

	virtual boolean isIdentified() {
		return true;
	}
	virtual boolean isUpgradable() {
		return false;
	}
	virtual String info() {
		return BPT::getText("lang.Amulet_Info");
			//"The Amulet of Yendor is the most powerful known artifact of unknown origin. It is said that the amulet " +
			//"is able to fulfil any wish if its owner's will-power is strong enough to \"persuade\" it to do it.";
	}
};