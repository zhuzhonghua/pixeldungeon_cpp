#include "bat.h"
#include "mobsprite.h"
#include "speck.h"

#include <algorithm>

CharSprite* Bat::Sprite()
{
	return new BatSprite();
}

Mob* Bat::CreateBat()
{
	return new Bat();
}

Bat::Bat()
{
	name = "vampire bat";
	//spriteClass = BatSprite.class;

	HP = HT = 30;
	_defenseSkill = 15;
	baseSpeed = 2.0f;

	EXP = 7;
	maxLvl = 15;

	flying = true;

	//loot = new PotionOfHealing();
	lootChance = 0.125f;
}

int Bat::damageRoll()
{
	return Random::NormalIntRange(6, 12);
}

int Bat::attackProc(Char* enemy, int damage)
{
	int reg = std::min(damage, HT - HP);

	if (reg > 0) 
	{
		HP += reg;
		sprite->emitter()->burst(Speck::factory(Speck::HEALING), 1);
	}

	return damage;
}

std::string Bat::description()
{
	return
		std::string("These brisk and tenacious inhabitants of cave domes may defeat much larger opponents by ") +
		std::string("replenishing their health with each successful attack.");
}
