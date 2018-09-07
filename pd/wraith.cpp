#include "wraith.h"
#include "mobsprite.h"
#include "bundle.h"
#include "level.h"
#include "dungeon.h"
#include "gamescene.h"
#include "alphatweener.h"

const float Wraith::SPAWN_DELAY = 2.0f;
const String Wraith::LEVEL = "level";

Wraith::Wraith()
{
	name = "wraith";
	//spriteClass = WraithSprite.class;

	HP = HT = 1;
	EXP = 0;

	flying = true;
}

CharSprite* Wraith::Sprite()
{
	return new WraithSprite();
}

Mob* Wraith::CreateWraith()
{
	return new Wraith();
}

void Wraith::storeInBundle(Bundle* bundle)
{
	Mob::storeInBundle(bundle);
	bundle->put(LEVEL, level);
}

void Wraith::restoreFromBundle(Bundle* bundle)
{
	Mob::restoreFromBundle(bundle);
	level = bundle->getInt(LEVEL);
	adjustStats(level);
}

int Wraith::damageRoll()
{
	return Random::NormalIntRange(1, 3 + level);
}

void Wraith::adjustStats(int level)
{
	this->level = level;
	_defenseSkill = attackSkill(NULL) * 5;
	enemySeen = true;
}

void Wraith::spawnAround(int pos)
{
	for (int i = 0; i < 4;i++){
	//for (int n : Level.NEIGHBOURS4) {
		int n = Level::NEIGHBOURS4[i];
		int cell = pos + n;
		if (Level::passable[cell] && Actor::findChar(cell) == NULL) {
			spawnAt(cell);
		}
	}
}

Wraith* Wraith::spawnAt(int pos)
{
	if (Level::passable[pos] && Actor::findChar(pos) == NULL) {

		Wraith* w = new Wraith();
		w->adjustStats(Dungeon::depth);
		w->pos = pos;
		w->state = w->HUNTING;
		GameScene::addMob(w, SPAWN_DELAY);

		w->sprite->alpha(0);
		w->sprite->parent->add(new AlphaTweener(w->sprite, 1, 0.5f));

		w->sprite->emitter()->burst(ShadowParticle::CURSE, 5);

		return w;
	}
	else {
		return NULL;
	}
}
