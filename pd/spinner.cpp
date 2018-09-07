#include "spinner.h"
#include "mobsprite.h"

CharSprite* Spinner::Sprite()
{
	return new SpinnerSprite();
}

Mob* Spinner::CreateSpinner()
{
	return new Spinner();
}

Spinner::Spinner()
{
	name = "cave spinner";
	//spriteClass = SpinnerSprite.class;

	HP = HT = 50;
	_defenseSkill = 14;

	EXP = 9;
	maxLvl = 16;

	//loot = new MysteryMeat();
	lootChance = 0.125f;

	//FLEEING = new Fleeing();
}

int Spinner::damageRoll()
{
	return Random::NormalIntRange(12, 16);
}

int Spinner::attackProc(Char* enemy, int damage)
{
	//if (Random.Int(2) == 0) {
	//	Buff.affect(enemy, Poison.class).set(Random.Int(7, 9) * Poison.durationFactor(enemy));
	//	state = FLEEING;
	//}

	return damage;
}

void Spinner::move(int step)
{
	//if (state == FLEEING) {
	//	GameScene.add(Blob.seed(pos, Random.Int(5, 7), Web.class));
	//}
	Mob::move(step);
}

bool Spinner::act()
{
	bool result = Mob::act();

	//if (state == FLEEING && buff(Terror.class) == null) 
	//{
	//	if (enemy != null && enemySeen && enemy.buff(Poison.class) == null) {
	//		state = HUNTING;
	//	}
	//}
	return result;
}

void Spinner::Fleeing::nowhereToRun()
{
	//if (buff(Terror.class) == null) {
	//	state = HUNTING;
	//}
	//else {
		Mob::Fleeing::nowhereToRun();
	//}
}
