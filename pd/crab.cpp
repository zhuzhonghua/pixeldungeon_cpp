#include "crab.h"
#include "mobsprite.h"
#include "util.h"

Crab::Crab()
{
	name = "sewer crab";
	//spriteClass = CrabSprite.class;

	HP = HT = 15;
	_defenseSkill = 5;
	baseSpeed = 2.0f;

	EXP = 3;
	maxLvl = 9;

	//loot = new MysteryMeat();
	lootChance = 0.167f;
}

Mob* Crab::CreateCrab()
{
	return new Crab();
}

CharSprite* Crab::Sprite()
{
	return new CrabSprite();
}

int Crab::damageRoll()
{
	return Random::NormalIntRange(3, 6);
}

void Crab::die(const std::string& cause)
{
	//Ghost.Quest.processSewersKill(pos);
	//super.die(cause);
}

std::string Crab::description()
{
	return
		std::string("These huge crabs are at the top of the food chain in the sewers. ") +
		std::string("They are extremely fast and their thick exoskeleton can withstand ") +
		std::string("heavy blows.");
}
