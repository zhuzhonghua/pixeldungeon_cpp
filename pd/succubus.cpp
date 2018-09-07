#include "succubus.h"
#include "mobsprite.h"
#include "speck.h"

CharSprite* Succubus::Sprite()
{
	return new SuccubusSprite();
}

Mob* Succubus::CreateSuccubus()
{
	return new Succubus();
}

Succubus::Succubus()
{
	name = "succubus";
	//spriteClass = SuccubusSprite.class;

	HP = HT = 80;
	_defenseSkill = 25;
	//viewDistance = Light.DISTANCE;

	EXP = 12;
	maxLvl = 25;

	//loot = new ScrollOfLullaby();
	lootChance = 0.05f;
}

int Succubus::damageRoll()
{
	return Random::NormalIntRange(15, 25);
}

int Succubus::attackProc(Char* enemy, int damage)
{
	if (Random::Int(3) == 0) 
	{
		//Buff.affect(enemy, Charm.class, Charm.durationFactor(enemy) * Random.IntRange(3, 7)).object = id();
		enemy->sprite->centerEmitter()->start(Speck::factory(Speck::HEART), 0.2f, 5);
		//Sample.INSTANCE.play(Assets.SND_CHARMS);
	}

	return damage;
}

