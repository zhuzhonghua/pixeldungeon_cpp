#include "heroclass.h"
#include "bpt.h"
#include "define.h"
#include "hero.h"

const char* HeroClass::WARRIOR = "lang.warrior";
const char* HeroClass::MAGE = "lang.mage";
const char* HeroClass::ROGUE = "lang.rogue";
const char* HeroClass::HUNTRESS = "lang.huntress";

const char* HeroClass::WAR_PERKS[] = {
	"lang.war_perks_1",
	"lang.war_perks_2",
	"lang.war_perks_3",
	"lang.war_perks_4",
	"lang.war_perks_5",
};

const char* HeroClass::MAG_PERKS[] = {
	"lang.mage_perks_1",
	"lang.mage_perks_2",
	"lang.mage_perks_3",
	"lang.mage_perks_4",
	"lang.mage_perks_5"
};

const char* HeroClass::ROG_PERKS[] = {
	"lang.rogue_perks_1",
	"lang.rogue_perks_2",
	"lang.rogue_perks_3",
	"lang.rogue_perks_4",
	"lang.rogue_perks_5",
	"lang.rogue_perks_6"
};

const char* HeroClass::HUN_PERKS[] = {
	"lang.huntress_perks_1",
	"lang.huntress_perks_2",
	"lang.huntress_perks_3",
	"lang.huntress_perks_4",
	"lang.huntress_perks_5"
};

const char* HeroClass::CLASS = "class";

void HeroClass::initCommon(Hero* hero)
{
	//(hero.belongings.armor = new ClothArmor()).identify();
	//new Food().identify().collect();
	//new Keyring().collect();
}

void HeroClass::initWarrior(Hero* hero)
{
	hero->STR = hero->STR + 1;

	//(hero.belongings.weapon = new ShortSword()).identify();
	//new Dart(8).identify().collect();
	//
	//QuickSlot.primaryValue = Dart.class;
	//
	//new PotionOfStrength().setKnown();
}

void HeroClass::initMage(Hero* hero)
{
	//(hero.belongings.weapon = new Knuckles()).identify();
	//
	//WandOfMagicMissile wand = new WandOfMagicMissile();
	//wand.identify().collect();
	//
	//QuickSlot.primaryValue = wand;
	//
	//new ScrollOfIdentify().setKnown();
}

void HeroClass::initRogue(Hero* hero)
{
	//(hero.belongings.weapon = new Dagger()).identify();
	//(hero.belongings.ring1 = new RingOfShadows()).upgrade().identify();
	//new Dart(8).identify().collect();
	//
	//hero.belongings.ring1.activate(hero);
	//
	//QuickSlot.primaryValue = Dart.class;
	//
	//new ScrollOfMagicMapping().setKnown();
}

void HeroClass::initHuntress(Hero* hero)
{
	hero->HP = (hero->HT -= 5);

	//(hero.belongings.weapon = new Dagger()).identify();
	//Boomerang boomerang = new Boomerang();
	//boomerang.identify().collect();
	//
	//QuickSlot.primaryValue = boomerang;
}

HeroClass::HeroClass(const std::string& title)
{
	this->_title = title;

	_type = Type::E_NONE;

	if (_title.compare(HeroClass::WARRIOR) == 0)
	{
		_type = Type::E_WARRIOR;
	}
	else if (_title.compare(HeroClass::MAGE) == 0)
	{
		_type = Type::E_MAGE;
	}
	else if (_title.compare(HeroClass::ROGUE) == 0)
	{
		_type = Type::E_ROGUE;
	}
	else if (_title.compare(HeroClass::HUNTRESS) == 0)
	{
		_type = Type::E_HUNTRESS;
	}	
}

HeroClass::HeroClass(const HeroClass& cl)
{
	this->_title = cl._title;
	this->_type = cl._type;
}

void HeroClass::initHero(Hero* hero)
{
	hero->heroClass = *this;

	initCommon(hero);

	switch (_type) {
	case E_WARRIOR:
		initWarrior(hero);
		break;

	case E_MAGE:
		initMage(hero);
		break;

	case E_ROGUE:
		initRogue(hero);
		break;

	case E_HUNTRESS:
		initHuntress(hero);
		break;
	}

	if (Badges::isUnlocked(masteryBadge())) 
	{
		//new TomeOfMastery().collect();
	}

	hero->updateAwareness();
}

std::string HeroClass::name()
{
	return BPT::getText(this->_title);
}

std::string HeroClass::spritesheet()
{
	if (_title.compare(HeroClass::WARRIOR) == 0)
	{
		return Assets::WARRIOR;
	}
	else if (_title.compare(HeroClass::MAGE) == 0)
	{
		return Assets::MAGE;
	}
	else if (_title.compare(HeroClass::ROGUE) == 0)
	{
		return Assets::ROGUE;
	}
	else if (_title.compare(HeroClass::HUNTRESS) == 0)
	{
		return Assets::HUNTRESS;
	}

	return "";
}

void HeroClass::perks(std::vector<std::string>& out)
{
	if (_title.compare(HeroClass::WARRIOR) == 0)
	{
		int len = sizeof(HeroClass::WAR_PERKS) / sizeof(char*);
		for (int i = 0; i < len; i++)
		{
			out.push_back(BPT::getText(HeroClass::WAR_PERKS[i]));
		}
	}
	else if (_title.compare(HeroClass::MAGE) == 0)
	{
		int len = sizeof(HeroClass::MAG_PERKS) / sizeof(char*);
		for (int i = 0; i < len; i++)
		{
			out.push_back(BPT::getText(HeroClass::MAG_PERKS[i]));
		}
	}
	else if (_title.compare(HeroClass::ROGUE) == 0)
	{
		int len = sizeof(HeroClass::ROG_PERKS) / sizeof(char*);
		for (int i = 0; i < len; i++)
		{
			out.push_back(BPT::getText(HeroClass::ROG_PERKS[i]));
		}
	}
	else if (_title.compare(HeroClass::HUNTRESS) == 0)
	{
		int len = sizeof(HeroClass::HUN_PERKS) / sizeof(char*);
		for (int i = 0; i < len; i++)
		{
			out.push_back(BPT::getText(HeroClass::HUN_PERKS[i]));
		}
	}
}

Badges::Badge HeroClass::masteryBadge()
{
	switch (this->_type) 
	{
	case E_WARRIOR:
		return Badges::MASTERY_WARRIOR;
	case E_MAGE:
		return Badges::MASTERY_MAGE;
	case E_ROGUE:
		return Badges::MASTERY_ROGUE;
	case E_HUNTRESS:
		return Badges::MASTERY_HUNTRESS;
	}
	return Badges::Badge("");
}

const std::string HeroSubClass::SUBCLASS = "subClass";

std::map<std::string, HeroSubClass> HeroSubClass::subClasses;

HeroSubClass HeroSubClass::NONE("NONE","","");
HeroSubClass HeroSubClass::GLADIATOR("GLADIATOR", "gladiator", 
	BPT::getText("lang.gladiator_desc"));
HeroSubClass HeroSubClass::BERSERKER("BERSERKER", "berserker",
	BPT::getText("lang.berserker_desc"));
HeroSubClass HeroSubClass::WARLOCK("WARLOCK", "warlock",
	BPT::getText("lang.warlock_desc"));
HeroSubClass HeroSubClass::BATTLEMAGE("BATTLEMAGE", "battlemage",
	BPT::getText("lang.battlemage_desc"));
HeroSubClass HeroSubClass::ASSASSIN("ASSASSIN", "assassin",
	BPT::getText("lang.assassin_desc"));
HeroSubClass HeroSubClass::FREERUNNER("FREERUNNER", "freerunner",
	BPT::getText("lang.freerunner_desc"));
HeroSubClass HeroSubClass::SNIPER("SNIPER", "sniper",
	BPT::getText("lang.sniper_desc"));
HeroSubClass HeroSubClass::WARDEN("WARDEN", "warden",
	BPT::getText("lang.warden_desc"));

void HeroSubClass::storeInBundle(Bundle* bundle)
{
	bundle->put(SUBCLASS, name);
}

HeroSubClass HeroSubClass::restoreInBundle(Bundle* bundle)
{
	std::string value = bundle->getString(SUBCLASS);
	return subClasses.find(value)->second;
}