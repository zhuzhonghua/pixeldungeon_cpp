#include "monk.h"
#include "mobsprite.h"
#include "dungeon.h"
#include "char.h"
#include "hero.h"

const std::string Monk::TXT_DISARM = "%s has knocked the %s from your hands!";

CharSprite* Monk::Sprite()
{
	return new MonkSprite();
}

Mob* Monk::CreateMonk()
{
	return new Monk();
}

Monk::Monk()
{
	name = "dwarf monk";
	//spriteClass = MonkSprite.class;

	HP = HT = 70;
	_defenseSkill = 30;

	EXP = 11;
	maxLvl = 21;

	//loot = new Food();
	lootChance = 0.083f;
}

int Monk::damageRoll()
{
	return Random::NormalIntRange(12, 16);
}

void Monk::die(const std::string& cause)
{
	//Imp.Quest.process(this);

	Mob::die(cause);
}

int Monk::attackProc(Char* enemy, int damage)
{
	if (Random::Int(6) == 0 && enemy == Dungeon::hero) 
	{
		Hero* hero = Dungeon::hero;
		//KindOfWeapon weapon = hero.belongings.weapon;
		//
		//if (weapon != null && !(weapon instanceof Knuckles) && !weapon.cursed) {
		//	hero.belongings.weapon = null;
		//	Dungeon.level.drop(weapon, hero.pos).sprite.drop();
		//	GLog.w(TXT_DISARM, name, weapon.name());
		//}
	}

	return damage;
}

