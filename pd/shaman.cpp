#include "shaman.h"
#include "mobsprite.h"
#include "level.h"
#include "dungeon.h"
#include "glog.h"
#include "camera.h"
#include "char.h"
#include "hero.h"

bool Shaman::canAttack(Char* enemy)
{
	return false;//Ballistica.cast( pos, enemy.pos, false, true ) == enemy.pos;
}

bool Shaman::doAttack(Char* enemy)
{
	if (Level::distance(pos, enemy->pos) <= 1) 
	{
		return Mob::doAttack(enemy);
	}
	else 
	{
		bool visible = Level::fieldOfView[pos] || Level::fieldOfView[enemy->pos];
		if (visible) 
		{
			((ShamanSprite*)sprite)->Zap(enemy->pos);
		}

		spend(TIME_TO_ZAP);

		if (hit(this, enemy, true)) 
		{
			int dmg = Random::Int(2, 12);
			if (Level::water[enemy->pos] && !enemy->flying) 
			{
				dmg *= 1.5f;
			}
			//enemy->damage(dmg, LightningTrap.LIGHTNING);

			enemy->sprite->centerEmitter()->burst(SparkParticle::FACTORY, 3);
			enemy->sprite->flash();

			if (enemy == Dungeon::hero) 
			{
				Camera::mainCamera->shake(2, 0.3f);

				if (!enemy->isAlive()) 
				{
					//Dungeon.fail(Utils.format(ResultDescriptions.MOB,
					//	Utils.indefinite(name), Dungeon.depth));
					GLog::n(TXT_LIGHTNING_KILLED, name.c_str());
				}
			}
		}
		else 
		{
			enemy->sprite->showStatus(CharSprite::NEUTRAL, enemy->defenseVerb());
		}

		return !visible;
	}
}

const float Shaman::TIME_TO_ZAP = 2.0f;
const std::string Shaman::TXT_LIGHTNING_KILLED = "%s's lightning bolt killed you...";

CharSprite* Shaman::Sprite()
{
	return new ShamanSprite();
}

Mob* Shaman::CreateShaman()
{
	return new Shaman();
}

Shaman::Shaman()
{
	name = "gnoll shaman";
	//spriteClass = ShamanSprite.class;

	HP = HT = 18;
	_defenseSkill = 8;

	EXP = 6;
	maxLvl = 14;

	//loot = Generator.Category.SCROLL;
	lootChance = 0.33f;
}

int Shaman::damageRoll()
{
	return Random::NormalIntRange(2, 6);
}

std::string Shaman::description()
{
	return
		std::string("The most intelligent gnolls can master shamanistic magic. Gnoll shamans prefer ") +
		std::string("battle spells to compensate for lack of might, not hesitating to use them ") +
		std::string("on those who question their status in a tribe.");
}
