#include "goo.h"
#include "mobsprite.h"
#include "dungeon.h"
#include "statistics.h"
#include "speck.h"
#include "glog.h"

const float Goo::PUMP_UP_DELAY = 2.0f;

CharSprite* Goo::Sprite()
{
	return new GooSprite();
}

Mob* Goo::CreateGoo()
{
	return new Goo();
}

Goo::Goo()
{
	pumpedUp = false;
	jumped = false;

	name = Dungeon::depth == Statistics::deepestFloor ? "Goo" : "spawn of Goo";

	HP = HT = 80;
	EXP = 10;
	_defenseSkill = 12;
	//spriteClass = GooSprite.class;

	//loot = new LloydsBeacon();
	lootChance = 0.333f;
}

int Goo::damageRoll()
{
	if (pumpedUp) 
	{
		return Random::NormalIntRange(5, 30);
	}
	else 
	{
		return Random::NormalIntRange(2, 12);
	}
}

bool Goo::act()
{
	if (Level::water[pos] && HP < HT) 
	{
		sprite->emitter()->burst(Speck::factory(Speck::HEALING), 1);
		HP++;
	}

	return Mob::act();
}

int Goo::attackProc(Char* enemy, int damage)
{
	if (Random::Int(3) == 0) 
	{
		//Buff.affect(enemy, Ooze.class);
		enemy->sprite->burst(0x000000, 5);
	}

	return damage;
}

void Goo::move(int step)
{
	//((SewerBossLevel)Dungeon.level).seal();
	Mob::move(step);
}

void Goo::die(const std::string& cause)
{
	Mob::die(cause);

	//((SewerBossLevel)Dungeon.level).unseal();

	//GameScene.bossSlain();
	//Dungeon.level.drop(new SkeletonKey(), pos).sprite.drop();

	//Badges::validateBossSlain();

	yell("glurp... glurp...");
}

void Goo::notice()
{
	Mob::notice();
	yell("GLURP-GLURP!");
}

std::string Goo::description()
{
	return
		std::string("Little known about The Goo. It's quite possible that it is not even a creature, but rather a ") +
		std::string("conglomerate of substances from the sewers that gained rudiments of free will.");
}

bool Goo::doAttack(Char* enemy)
{
	if (pumpedUp) 
	{
		if (Level::adjacent(pos, enemy->pos)) 
		{
			// Pumped up attack WITHOUT accuracy penalty
			jumped = false;
			return Mob::doAttack(enemy);
		}
		else 
		{
			// Pumped up attack WITH accuracy penalty
			jumped = true;
			//if (Ballistica.cast(pos, enemy.pos, false, true) == enemy.pos) {
			//	final int dest = Ballistica.trace[Ballistica.distance - 2];
			//
			//	Callback afterJump = new Callback(){
			//		@Override
			//		public void call() {
			//			move(dest);
			//			Dungeon.level.mobPress(Goo.this);
			//			Goo.super.doAttack(enemy);
			//		}
			//	};
			//
			//	if (Dungeon.visible[pos] || Dungeon.visible[dest]) {
			//
			//		sprite.jump(pos, dest, afterJump);
			//		return false;
			//
			//	}
			//	else {
			//
			//		afterJump.call();
			//		return true;
			//
			//	}
			//}
			//else {

				sprite->Idle();
				pumpedUp = false;
				return true;
			//}
		}

	}
	else if (Random::Int(3) > 0) 
	{
		// Normal attack
		return Mob::doAttack(enemy);
	}
	else 
	{
		// Pumping up
		pumpedUp = true;
		spend(PUMP_UP_DELAY);

		((GooSprite*)sprite)->pumpUp();

		if (Dungeon::visible[pos]) 
		{
			sprite->showStatus(CharSprite::NEGATIVE, "!!!");
			GLog::n("Goo is pumping itself up!");
		}

		return true;
	}
}
