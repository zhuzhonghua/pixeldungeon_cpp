#include "golem.h"
#include "mobsprite.h"

CharSprite* Golem::Sprite()
{
	return new GolemSprite();
}

Mob* Golem::CreateGolem()
{
	return new Golem();
}

Golem::Golem()
{
	name = "golem";
	//spriteClass = GolemSprite.class;

	HP = HT = 85;
	_defenseSkill = 18;

	EXP = 12;
	maxLvl = 22;
}

int Golem::damageRoll()
{
	return Random::NormalIntRange(20, 40);
}

void Golem::die(const std::string& cause)
{
	//Imp.Quest.process(this);

	Mob::die(cause);
}
