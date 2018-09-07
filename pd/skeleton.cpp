#include "skeleton.h"
#include "mobsprite.h"
#include "util.h"
#include "level.h"
#include "dungeon.h"
#include "glog.h"
#include <algorithm>
#include "char.h"
#include "hero.h"
#include "char.h"

const std::string Skeleton::TXT_HERO_KILLED = "You were killed by the explosion of bones...";

Skeleton::Skeleton()
{
	name = "skeleton";
	//spriteClass = SkeletonSprite.class;

	HP = HT = 25;
	_defenseSkill = 9;

	EXP = 5;
	maxLvl = 10;
}

Mob* Skeleton::CreateSkeleton()
{
	return new Skeleton();
}

CharSprite* Skeleton::Sprite()
{
	return new SkeletonSprite();
}

int Skeleton::damageRoll()
{
	return Random::NormalIntRange(3, 8);
}

void Skeleton::die(const std::string& cause)
{
	Mob::die(cause);

	bool heroKilled = false;
	for (int i = 0; i < 8; i++) 
	{
		Char* ch = findChar(pos + Level::NEIGHBOURS8[i]);
		if (ch != NULL && ch->isAlive()) 
		{
			int damage = std::max(0, damageRoll() - Random::IntRange(0, ch->dr() / 2));
			//ch->damage(damage, this);
			if (ch == Dungeon::hero && !ch->isAlive()) 
			{
				heroKilled = true;
			}
		}
	}

	if (Dungeon::visible[pos]) 
	{
		//Sample.INSTANCE.play(Assets.SND_BONES);
	}

	if (heroKilled) 
	{
		//Dungeon::fail(Utils.format(ResultDescriptions.MOB, Utils.indefinite(name), Dungeon::depth));
		GLog::n(TXT_HERO_KILLED);
	}
}
