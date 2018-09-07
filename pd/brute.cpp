#include "brute.h"
#include "mobsprite.h"
#include "dungeon.h"
#include "glog.h"

const std::string Brute::TXT_ENRAGED = "%s becomes enraged!";

CharSprite* Brute::Sprite()
{
	return new BruteSprite();
}

void Brute::restoreFromBundle(Bundle* bundle)
{
	Mob::restoreFromBundle(bundle);
	enraged = HP < HT / 4;
}

Mob* Brute::CreateBrute()
{
	return new Brute();
}

Brute::Brute()
{
	name = "gnoll brute";
	//spriteClass = BruteSprite.class;

	HP = HT = 40;
	_defenseSkill = 15;

	EXP = 8;
	maxLvl = 15;

	//loot = Gold.class;
	lootChance = 0.5f;

	enraged = false;
}

int Brute::damageRoll()
{
	return enraged ?
		Random::NormalIntRange(10, 40) :
		Random::NormalIntRange(8, 18);
}

void Brute::damage(int dmg, const std::string& src)
{
	Mob::damage(dmg, src);

	if (isAlive() && !enraged && HP < HT / 4) 
	{
		enraged = true;
		spend(TICK);
		if (Dungeon::visible[pos]) 
		{
			GLog::w(TXT_ENRAGED, name.c_str());
			sprite->showStatus(CharSprite::NEGATIVE, "enraged");
		}
	}
}

std::string Brute::description()
{
	return
		std::string("Brutes are the largest, strongest and toughest of all gnolls. When severely wounded, ") +
		std::string("they go berserk, inflicting even more damage to their enemies.");
}
