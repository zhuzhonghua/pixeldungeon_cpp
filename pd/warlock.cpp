#include "warlock.h"
#include "mobsprite.h"
#include "level.h"
#include "dungeon.h"
#include "glog.h"
#include "char.h"
#include "hero.h"

CharSprite* Warlock::Sprite()
{
	return new WarlockSprite();
}

Mob* Warlock::CreateWarlock()
{
	return new Warlock();
}

Warlock::Warlock()
{
	name = "dwarf warlock";
	//spriteClass = WarlockSprite.class;

	HP = HT = 70;
	_defenseSkill = 18;

	EXP = 11;
	maxLvl = 21;

	//loot = Generator.Category.POTION;
	lootChance = 0.83f;
}

int Warlock::damageRoll()
{
	return Random::NormalIntRange(12, 20);
}

bool Warlock::canAttack(Char* enemy)
{
	return true;// Ballistica.cast(pos, enemy.pos, false, true) == enemy.pos;
}

bool Warlock::doAttack(Char* enemy)
{
	if (Level::adjacent(pos, enemy->pos)) 
	{
		return Mob::doAttack(enemy);
	}
	else 
	{
		bool visible = Level::fieldOfView[pos] || Level::fieldOfView[enemy->pos];
		if (visible) 
		{
			((WarlockSprite*)sprite)->Zap(enemy->pos);
		}
		else 
		{
			zap();
		}

		return !visible;
	}
}

const float Warlock::TIME_TO_ZAP = 1.0f;

const std::string Warlock::TXT_SHADOWBOLT_KILLED = "%s's shadow bolt killed you...";

void Warlock::zap()
{
	spend(TIME_TO_ZAP);

	if (hit(this, enemy, true)) {
		if (enemy == Dungeon::hero && Random::Int(2) == 0) 
		{
			//Buff.prolong(enemy, Weakness.class, Weakness.duration(enemy));
		}

		int dmg = Random::Int(12, 18);
		//enemy->damage(dmg, this);

		if (!enemy->isAlive() && enemy == Dungeon::hero) 
		{
			//Dungeon.fail(Utils.format(ResultDescriptions.MOB,
			//	Utils.indefinite(name), Dungeon.depth));
			GLog::n(TXT_SHADOWBOLT_KILLED, name);
		}
	}
	else 
	{
		enemy->sprite->showStatus(CharSprite::NEUTRAL, enemy->defenseVerb());
	}
}

