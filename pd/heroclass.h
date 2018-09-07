#pragma once

#include <string>
#include <vector>

#include "badge.h"

class Hero;

class HeroClass{
public:
	enum Type{		
		E_WARRIOR,
		E_MAGE,
		E_ROGUE,
		E_HUNTRESS,
		E_NONE
	};
	static const char* WARRIOR;
	static const char* MAGE;
	static const char* ROGUE;
	static const char* HUNTRESS;
private:
	static const char* WAR_PERKS[];
	static const char* MAG_PERKS[];
	static const char* ROG_PERKS[];
	static const char* HUN_PERKS[];

	static const char* CLASS;

	std::string _title;
	Type _type;

	static void initCommon(Hero* hero);
	static void initWarrior(Hero* hero);
	static void initMage(Hero* hero);
	static void initRogue(Hero* hero);
	static void initHuntress(Hero* hero);
public:
	HeroClass(const std::string& title);
	HeroClass(const HeroClass& cl);

	void initHero(Hero* hero);
	std::string title() { return _title; }
	Type type() const { return _type; }
	std::string name();
	std::string spritesheet();
	void perks(std::vector<std::string>& out);
	void storeInBundle(/*Bundle bundle*/);
	static HeroClass restoreInBundle(/*Bundle bundle*/);
	int ordinal() { return _type; }
	bool isNull() { return _type == E_NONE; }
	void setNull() { _type == E_NONE; _title = ""; }

	bool operator < (const HeroClass &c) const
	{
		return _type < c._type;
	}

	bool operator == (const HeroClass &c) const
	{
		return _type == c._type && _title.compare(c._title) == 0;
	}

	Badges::Badge masteryBadge();
};

#include "bundle.h"

class HeroSubClass{
private:
	std::string title;
	std::string desc;
	std::string name;

	static const std::string SUBCLASS;

	HeroSubClass(const std::string& name,  const std::string& title, const std::string& desc)
	{
		this->title = title;
		this->desc = desc;

		this->name = name;

		subClasses.insert(std::make_pair(name, *this));
	}

public:
	static std::map<std::string, HeroSubClass> subClasses;

	static HeroSubClass NONE;
	static HeroSubClass GLADIATOR;
	static HeroSubClass BERSERKER;
	static HeroSubClass WARLOCK;
	static HeroSubClass BATTLEMAGE;
	static HeroSubClass ASSASSIN;
	static HeroSubClass FREERUNNER;
	static HeroSubClass SNIPER;
	static HeroSubClass WARDEN;

public:
	std::string Title() { return title; }
	std::string Desc() { return desc; }

	void storeInBundle(Bundle* bundle);

	static HeroSubClass restoreInBundle(Bundle* bundle);

	bool operator == (const HeroSubClass &c) const
	{
		return name.compare(c.name) == 0;
	}
	bool operator != (const HeroSubClass &c) const
	{
		return name.compare(c.name) != 0;
	}
};