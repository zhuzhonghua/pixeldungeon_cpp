#include "scorpio.h"
#include "mobsprite.h"


CharSprite* Scorpio::Sprite()
{
	return new ScorpioSprite();
}

Mob* Scorpio::CreateScorpio()
{
	return new Scorpio();
}

Scorpio::Scorpio()
{
	name = "scorpio";
	//spriteClass = ScorpioSprite.class;

	HP = HT = 95;
	_defenseSkill = 24;
	//viewDistance = Light.DISTANCE;

	EXP = 14;
	maxLvl = 25;

	//loot = new PotionOfHealing();
	lootChance = 0.125f;
}

int Scorpio::damageRoll()
{
	return Random::NormalIntRange(20, 32);
}

int Scorpio::attackProc(Char* enemy, int damage)
{
	//if (Random.Int(2) == 0) {
	//	Buff.prolong(enemy, Cripple.class, Cripple.DURATION);
	//}

	return damage;
}

bool Scorpio::canAttack(Char* enemy)
{
	return true;// !Level.adjacent(pos, enemy.pos) && Ballistica.cast(pos, enemy.pos, false, true) == enemy.pos;
}

bool Scorpio::getCloser(int target)
{
	if (state == HUNTING) 
	{
		return enemySeen && getFurther(target);
	}
	else 
	{
		return Mob::getCloser(target);
	}
}

void Scorpio::dropLoot()
{
	//if (Random.Int(8) == 0) {
	//	Dungeon.level.drop(new PotionOfHealing(), pos).sprite.drop();
	//}
	//else if (Random.Int(6) == 0) {
	//	Dungeon.level.drop(new MysteryMeat(), pos).sprite.drop();
	//}
}
