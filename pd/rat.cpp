#include "rat.h"
#include "util.h"
#include "mobsprite.h"

Rat::Rat()
{
	name = "marsupial rat";
	//spriteClass = RatSprite.class;

	HP = HT = 8;
	_defenseSkill = 3;

	maxLvl = 5;
}

Mob* Rat::CreateRat()
{
	return new Rat();
}

CharSprite* Rat::Sprite()
{
	return new RatSprite();
}

int Rat::damageRoll()
{
	return Random::NormalIntRange(1, 5);
}

void Rat::die(const std::string& cause)
{
	//Ghost.Quest.processSewersKill(pos);
	//
	//super.die(cause);
}

std::string Rat::description()
{
	return
		std::string("Marsupial rats are aggressive, but rather weak denizens ") +
		std::string("of the sewers. They can be dangerous only in big numbers.");
}
