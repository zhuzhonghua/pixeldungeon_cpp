#include "gnoll.h"
#include "mobsprite.h"
#include "util.h"

Gnoll::Gnoll()
{
	name = "gnoll scout";
	//spriteClass = GnollSprite.class;

	HP = HT = 12;
	_defenseSkill = 4;

	EXP = 2;
	maxLvl = 8;

	//loot = Gold.class;
	lootChance = 0.5f;
}

Mob* Gnoll::CreateGnoll()
{
	return new Gnoll();
}

CharSprite* Gnoll::Sprite()
{
	return new GnollSprite();
}

int Gnoll::damageRoll()
{
	return Random::NormalIntRange(2, 5);
}

void Gnoll::die(const std::string& cause)
{
	//Ghost.Quest.processSewersKill(pos);
	//super.die(cause);
}

std::string Gnoll::description()
{
	return
		std::string("Gnolls are hyena-like humanoids. They dwell in sewers and dungeons, venturing up to raid the surface from time to time. ") +
		std::string("Gnoll scouts are regular members of their pack, they are not as strong as brutes and not as intelligent as shamans.");
}
