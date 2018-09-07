#include "yog.h"
#include "mobsprite.h"
#include "dungeon.h"
#include "statistics.h"
#include "glog.h"
#include "gamescene.h"
#include "pushing.h"
#include "level.h"
#include "key.h"
#include "heap.h"
#include "simpleresource.h"
#include "blob.h"
#include "hero.h"

REFLECMOB(Yog);

CharSprite* Yog::Sprite()
{
	return new YogSprite();
}

Mob* Yog::CreateYog()
{
	return new Yog();
}

Yog::Yog()
{
	name = Dungeon::depth == Statistics::deepestFloor ? BPT::getText("lang.Yog_Dzewa") : BPT::getText("lang.echo_Yog_Dzewa");
	//spriteClass = YogSprite.class;

	HP = HT = 300;

	EXP = 50;

	state = PASSIVE;
}

void Yog::spawnFists()
{
	RottingFist* fist1 = new RottingFist();
	BurningFist* fist2 = new BurningFist();

	do {
		fist1->pos = pos + Level::NEIGHBOURS8[Random::Int(8)];
		fist2->pos = pos + Level::NEIGHBOURS8[Random::Int(8)];
	} while (!Level::passable[fist1->pos] || !Level::passable[fist2->pos] || fist1->pos == fist2->pos);

	GameScene::addMob(fist1);
	GameScene::addMob(fist2);
}

void Yog::damage(int dmg, const std::string& src)
{
	if (fistsCount > 0) 
	{
		for (std::set<Mob*>::iterator itr = Dungeon::level->mobs.begin();
			itr != Dungeon::level->mobs.end(); itr++)
		{
			Mob* mob = *itr;
			if (dynamic_cast<BurningFist*>(mob) != NULL ||
				dynamic_cast<RottingFist*>(mob) != NULL)
			{
				mob->beckon(pos);
			}
		}

		dmg >>= fistsCount;
	}

	Mob::damage(dmg, src);
}

int Yog::defenseProc(Char* enemy, int damage)
{
	std::list<int> spawnPoints;

	for (int i = 0; i < 8; i++) {
		int p = pos + Level::NEIGHBOURS8[i];
		if (Actor::findChar(p) == NULL && (Level::passable[p] || Level::avoid[p])) 
		{
			spawnPoints.push_back(p);
		}
	}

	if (spawnPoints.size() > 0) 
	{
		Larva* larva = new Larva();
		larva->pos = RandomT<int>::element(spawnPoints);

		GameScene::addMob(larva);
		Actor::addDelayed(new Pushing(larva, pos, larva->pos), -1);
	}

	return Mob::defenseProc(enemy, damage);
}

void Yog::die(const std::string& cause)
{
	for (std::set<Mob*>::iterator itr = Dungeon::level->mobs.begin();
		itr != Dungeon::level->mobs.end(); itr++)
	{
		Mob* mob = *itr;
		if (dynamic_cast<BurningFist*>(mob) != NULL ||
			dynamic_cast<RottingFist*>(mob) != NULL)
		{
			mob->die(cause);
		}
	}

	GameScene::bossSlain();
	Dungeon::level->drop(new SkeletonKey(), pos)->sprite->drop();
	Mob::die(cause);

	yell("...");
}

void Yog::notice()
{
	Mob::notice();
	yell("Hope is an illusion...");
}

const std::string Yog::TXT_DESC = BPT::getText("lang.yog_desc");

int Yog::fistsCount = 0;

CharSprite* Yog::RottingFist::Sprite()
{
	return new RottingFistSprite();
}

Mob* Yog::RottingFist::CreateRottingFist()
{
	return new RottingFist();
}

Yog::RottingFist::RottingFist()
{
	name = BPT::getText("lang.rotting_fist");
	//spriteClass = RottingFistSprite.class;

	HP = HT = 300;
	_defenseSkill = 25;

	EXP = 0;

	state = WANDERING;

	fistsCount++;
}

void Yog::RottingFist::die(const std::string& cause)
{
	Mob::die(cause);
	fistsCount--;
}

int Yog::RottingFist::damageRoll()
{
	return Random::NormalIntRange(24, 36);
}

int Yog::RottingFist::attackProc(Char* enemy, int damage)
{
	if (Random::Int(3) == 0) 
	{
		Buff::affect(enemy, "Ooze");
		enemy->sprite->burst(0xFF000000, 5);
	}

	return damage;
}

bool Yog::RottingFist::act()
{
	if (Level::water[pos] && HP < HT) {
		sprite->emitter()->burst(ShadowParticle::UP, 2);
		HP += REGENERATION;
	}

	return Mob::act();
}

CharSprite* Yog::BurningFist::Sprite()
{
	return new BurningFistSprite();
}

Mob* Yog::BurningFist::CreateBurningFist()
{
	return new BurningFist();
}

Yog::BurningFist::BurningFist()
{
	name = BPT::getText("lang.burning_fist");;
	//spriteClass = BurningFistSprite.class;

	HP = HT = 200;
	_defenseSkill = 25;

	EXP = 0;

	state = WANDERING;

	fistsCount++;
}

void Yog::BurningFist::die(const std::string& cause)
{
	Mob::die(cause);
	fistsCount--;
}

int Yog::BurningFist::damageRoll()
{
	return Random::NormalIntRange(20, 32);
}

bool Yog::BurningFist::attack(Char* enemy)
{
	if (!Level::adjacent(pos, enemy->pos)) 
	{
		spend(attackDelay());

		if (hit(this, enemy, true)) 
		{
			int dmg = damageRoll();
			enemy->damage(dmg, this->getClassName());

			enemy->sprite->bloodBurstA(sprite->center(), dmg);
			enemy->sprite->flash();

			if (!enemy->isAlive() && enemy == Dungeon::hero) 
			{
				Dungeon::fail(GameMath::format(ResultDescriptions::BOSS.c_str(), name, Dungeon::depth));
				GLog::n(TXT_KILL, name);
			}
			return true;
		}
		else 
		{
			enemy->sprite->showStatus(CharSprite::NEUTRAL, enemy->defenseVerb());
			return false;
		}
	}
	else 
	{
		return Mob::attack(enemy);
	}
}

bool Yog::BurningFist::act()
{
	for (int i = 0; i < 9/*Level.NEIGHBOURS9.length*/; i++) {
		GameScene::addBlob(Blob::seed(pos + Level::NEIGHBOURS9[i], 2, "BlobFire"));
	}

	return Mob::act();
}

bool Yog::BurningFist::canAttack(Char* enemy)
{
	return Ballistica::cast(pos, enemy->pos, false, true) == enemy->pos;
}

CharSprite* Yog::Larva::Sprite()
{
	return new LarvaSprite();
}

Mob* Yog::Larva::CreateLarva()
{
	return new Larva();
}

Yog::Larva::Larva()
{
	name = BPT::getText("lang.gods_larva");;
	//spriteClass = LarvaSprite.class;

	HP = HT = 25;
	_defenseSkill = 20;

	EXP = 0;

	state = HUNTING;
}

int Yog::Larva::damageRoll()
{
	return Random::NormalIntRange(15, 20);
}
