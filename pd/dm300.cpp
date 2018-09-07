#include "dm300.h"
#include "mobsprite.h"
#include "dungeon.h"
#include "statistics.h"
#include "terrain.h"
#include "glog.h"
#include "cellemitter.h"
#include "speck.h"
#include "gamescene.h"
#include "hero.h"

CharSprite* DM300::Sprite()
{
	return new DM300Sprite();
}

Mob* DM300::CreateDM300()
{
	return new DM300();
}

DM300::DM300()
{
	name = Dungeon::depth == Statistics::deepestFloor ? "DM-300" : "DM-350";
	//spriteClass = DM300Sprite.class;

	HP = HT = 200;
	EXP = 30;
	_defenseSkill = 18;

	//loot = new RingOfThorns().random();
	lootChance = 0.333f;
}

int DM300::damageRoll()
{
	return Random::NormalIntRange(18, 24);
}

bool DM300::act()
{
	//GameScene.add(Blob.seed(pos, 30, ToxicGas.class));
	return Mob::act();
}

void DM300::move(int step)
{
	Mob::move(step);

	if (Dungeon::level->map[step] == Terrain::INACTIVE_TRAP && HP < HT) 
	{
		HP += Random::Int(1, HT - HP);
		//sprite->emitter()->burst(ElmoParticle.FACTORY, 5);

		if (Dungeon::visible[step] && Dungeon::hero->isAlive()) 
		{
			GLog::n("DM-300 repairs itself!");
		}
	}

	int cells[] = {
		step - 1, step + 1, step - Level::WIDTH, step + Level::WIDTH,
		step - 1 - Level::WIDTH,
		step - 1 + Level::WIDTH,
		step + 1 - Level::WIDTH,
		step + 1 + Level::WIDTH
	};
	int cell = cells[Random::Int(sizeof(cells)/sizeof(int))];

	if (Dungeon::visible[cell]) 
	{
		CellEmitter::get(cell)->start(Speck::factory(Speck::ROCK), 0.07f, 10);
		Camera::mainCamera->shake(3, 0.7f);
		//Sample.INSTANCE.play(Assets.SND_ROCKS);

		if (Level::water[cell]) 
		{
			GameScene::ripple(cell);
		}
		else if (Dungeon::level->map[cell] == Terrain::EMPTY) 
		{
			Level::set(cell, Terrain::EMPTY_DECO);
			GameScene::updateMap(cell);
		}
	}

	Char* ch = Actor::findChar(cell);
	if (ch != NULL && ch != this) 
	{
		//Buff.prolong(ch, Paralysis.class, 2);
	}
}

void DM300::die(const std::string& cause)
{
	Mob::die(cause);

	GameScene::bossSlain();
	//Dungeon::level->drop(new SkeletonKey(), pos).sprite.drop();

	//Badges::validateBossSlain();

	yell("Mission failed. Shutting down.");
}

void DM300::notice()
{
	Mob::notice();
	yell("Unauthorised personnel detected.");
}
