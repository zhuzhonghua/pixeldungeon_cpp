#include "swarm.h"
#include "mobsprite.h"
#include "bundle.h"
#include "util.h"
#include "level.h"
#include "dungeon.h"
#include "terrain.h"
#include "gamescene.h"

const float Swarm::SPLIT_DELAY = 1.0f;
const std::string Swarm::GENERATION = "generation";

Swarm* Swarm::split()
{
	Swarm* clone = new Swarm();
	clone->generation = generation + 1;
	//if (buff(Burning.class) != null) {
	//	Buff.affect(clone, Burning.class).reignite(clone);
	//}
	//if (buff(Poison.class) != null) {
	//	Buff.affect(clone, Poison.class).set(2);
	//}
	return clone;
}

Swarm::Swarm()
{
	name = "swarm of flies";
	//spriteClass = SwarmSprite.class;

	HP = HT = 80;
	_defenseSkill = 5;

	maxLvl = 10;

	flying = true;

	generation = 0;
}

Mob* Swarm::CreateSwarm()
{
	return new Swarm();
}

CharSprite* Swarm::Sprite()
{
	return new SwarmSprite();
}

void Swarm::storeInBundle(Bundle* bundle)
{
	Mob::storeInBundle(bundle);
	bundle->put(GENERATION, generation);
}

void Swarm::restoreFromBundle(Bundle* bundle)
{
	Mob::restoreFromBundle(bundle);
	generation = bundle->getInt(GENERATION);
}

int Swarm::damageRoll()
{
	return Random::NormalIntRange(1, 4);
}

int Swarm::defenseProc(Char* enemy, int damage)
{
	if (HP >= damage + 2) 
	{
		std::list<int> candidates;
		std::vector<bool> passable = Level::passable;

		int neighbours[] = { pos + 1, pos - 1, pos + Level::WIDTH, pos - Level::WIDTH };
		for (int i = 0; i < sizeof(neighbours) / sizeof(int);i++)
		{
			int n = neighbours[i];

			if (passable[n] && Actor::findChar(n) == NULL) 
			{
				candidates.push_back(n);
			}
		}

		if (candidates.size() > 0) 
		{
			Swarm* clone = split();
			clone->HP = (HP - damage) / 2;
			clone->pos = RandomT<int>::element(candidates);
			clone->state = clone->HUNTING;

			if (Dungeon::level->map[clone->pos] == Terrain::DOOR) 
			{
				//Door.enter(clone->pos);
			}

			GameScene::addMob(clone, SPLIT_DELAY);
			//Actor::addDelayed(new Pushing(clone, pos, clone->pos), -1);

			HP -= clone->HP;
		}
	}

	return damage;
}

std::string Swarm::description()
{
	return
		std::string("The deadly swarm of flies buzzes angrily. Every non-magical attack ") +
		std::string("will split it into two smaller but equally dangerous swarms.");
}
