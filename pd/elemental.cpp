#include "elemental.h"
#include "mobsprite.h"

CharSprite* Elemental::Sprite()
{
	return new ElementalSprite();
}

Mob* Elemental::CreateElemental()
{
	return new Elemental();
}

Elemental::Elemental()
{
	name = "fire elemental";
	//spriteClass = ElementalSprite.class;

	HP = HT = 65;
	_defenseSkill = 20;

	EXP = 10;
	maxLvl = 20;

	flying = true;

	//loot = new PotionOfLiquidFlame();
	lootChance = 0.1f;
}

int Elemental::damageRoll()
{
	return Random::NormalIntRange(16, 20);
}

int Elemental::attackProc(Char* enemy, int damage)
{
	//if (Random.Int(2) == 0) {
	//	Buff.affect(enemy, Burning.class).reignite(enemy);
	//}

	return damage;
}

void Elemental::add(Buff* buff)
{
	//if (buff instanceof Burning) {
	//	if (HP < HT) {
	//		HP++;
	//		sprite.emitter().burst(Speck.factory(Speck.HEALING), 1);
	//	}
	//}
	//else {
	//	if (buff instanceof Frost) {
	//		damage(Random.NormalIntRange(1, HT * 2 / 3), buff);
	//	}
	//	super.add(buff);
	//}
}

