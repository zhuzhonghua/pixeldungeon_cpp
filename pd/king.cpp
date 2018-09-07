#include "king.h"
#include "mobsprite.h"
#include "dungeon.h"
#include "statistics.h"
#include "speck.h"
#include "level.h"
#include "pathfinder.h"
#include "gamescene.h"
#include "hero.h"

int King::Undead::count = 0;

CharSprite* King::Undead::Sprite()
{
	return new UndeadSprite();
}

Mob* King::Undead::CreateUndead()
{
	return new Undead();
}

King::Undead::Undead()
{
	name = "undead dwarf";
	//spriteClass = UndeadSprite.class;

	HP = HT = 28;
	_defenseSkill = 15;

	EXP = 0;

	state = WANDERING;
}

int King::Undead::damageRoll()
{
	return Random::NormalIntRange(12, 16);
}

int King::Undead::attackProc(Char* enemy, int damage)
{
	if (Random::Int(MAX_ARMY_SIZE) == 0) 
	{
		//Buff.prolong(enemy, Paralysis.class, 1);
	}

	return damage;
}

void King::Undead::damage(int dmg, const std::string& src)
{
	Mob::damage(dmg, src);
	//if (src instanceof ToxicGas) {
	//	((ToxicGas)src).clear(pos);
	//}
}

void King::Undead::die(const std::string& cause)
{
	Mob::die(cause);

	if (Dungeon::visible[pos]) 
	{
		//Sample.INSTANCE.play(Assets.SND_BONES);
	}
}

void King::Undead::onAdd()
{
	count++;
	Mob::onAdd();
}

void King::Undead::onRemove()
{
	count--;
	Mob::onRemove();
}

CharSprite* King::Sprite()
{
	return new KingSprite();
}

Mob* King::CreateKing()
{
	return new King();
}

King::King()
{
	nextPedestal = true;

	name = Dungeon::depth == Statistics::deepestFloor ? "King of Dwarves" : "undead King of Dwarves";
	//spriteClass = KingSprite.class;

	HP = HT = 300;
	EXP = 40;
	_defenseSkill = 25;

	Undead::count = 0;
}

void King::storeInBundle(Bundle* bundle)
{
	Mob::storeInBundle(bundle);
	bundle->put(PEDESTAL, nextPedestal);
}

void King::restoreFromBundle(Bundle* bundle)
{
	Mob::restoreFromBundle(bundle);
	nextPedestal = bundle->getBoolean(PEDESTAL);
}

int King::damageRoll()
{
	return Random::NormalIntRange(20, 38);
}

bool King::attack(Char* enemy)
{
	//if (canTryToSummon() && pos == CityBossLevel.pedestal(nextPedestal)) {
	//	summon();
	//	return true;
	//}
	//else {
	//	if (Actor.findChar(CityBossLevel.pedestal(nextPedestal)) == enemy) {
	//		nextPedestal = !nextPedestal;
	//	}
		return Mob::attack(enemy);
	//}
}

void King::die(const std::string& cause)
{
	//GameScene.bossSlain();
	//Dungeon.level.drop(new ArmorKit(), pos).sprite.drop();
	//Dungeon.level.drop(new SkeletonKey(), pos).sprite.drop();

	Mob::die(cause);

	//Badges.validateBossSlain();

	yell("You cannot kill me, " + Dungeon::hero->heroClass.title() + "... I am... immortal...");
}

void King::notice()
{
	Mob::notice();
	yell("How dare you!");
}

bool King::getCloser(int target)
{
	return //canTryToSummon() ?
		//super.getCloser(CityBossLevel.pedestal(nextPedestal)) :
		Mob::getCloser(target);
}

bool King::canAttack(Char* enemy)
{
	return //canTryToSummon() ?
		//pos == CityBossLevel.pedestal(nextPedestal) :
		Level::adjacent(pos, enemy->pos);
}

const std::string King::PEDESTAL = "pedestal";

bool King::canTryToSummon()
{
	//if (Undead::count < maxArmySize()) 
	//{
	//	Char ch = Actor.findChar(CityBossLevel.pedestal(nextPedestal));
	//	return ch == this || ch == null;
	//}
	//else {
		return false;
	//}
}

int King::maxArmySize()
{
	return 1 + MAX_ARMY_SIZE * (HT - HP) / HT;
}

void King::summon()
{
	nextPedestal = !nextPedestal;

	sprite->centerEmitter()->start(Speck::factory(Speck::SCREAM), 0.4f, 2);
	//Sample.INSTANCE.play(Assets.SND_CHALLENGE);

	std::vector<bool> passable = Level::passable;
	for (std::set<Actor*>::iterator itr = Actor::all.begin();
		itr != Actor::all.end(); itr++)
	{
		Actor* actor = *itr;
		if (dynamic_cast<Char*>(actor) != NULL)
		{
			passable[((Char*)actor)->pos] = false;
		}
	}

	int undeadsToSummon = maxArmySize() - Undead::count;
	PathFinder::buildDistanceMap(pos, passable, undeadsToSummon);
	PathFinder::distance[pos] = std::numeric_limits<int>::max();
	int dist = 1;

undeadLabel:
	for (int i = 0; i < undeadsToSummon; i++) {
		do {
			for (int j = 0; j < Level::LENGTH; j++) {
				if (PathFinder::distance[j] == dist) 
				{
					Undead* undead = new Undead();
					undead->pos = j;
					GameScene::addMob(undead);

					//WandOfBlink.appear(undead, j);
					//new Flare(3, 32).color(0x000000, false).show(undead.sprite, 2f);

					PathFinder::distance[j] = std::numeric_limits<int>::max();

					goto undeadLabel;
				}
			}
			dist++;
		} while (dist < undeadsToSummon);
	}

	yell("Arise, slaves!");
}
