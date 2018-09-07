#include "mimic.h"
#include "mobsprite.h"
#include "dungeon.h"
#include "gold.h"
#include "level.h"
#include "heap.h"
#include "gamescene.h"
#include "cellemitter.h"
#include "speck.h"
#include "hero.h"

const String Mimic::LEVEL = "level";
const String Mimic::ITEMS = "items";

Mimic::Mimic()
{
	name = "mimic";
	//spriteClass = MimicSprite.class;
}

CharSprite* Mimic::Sprite()
{
	return new MimicSprite();
}

Mob* Mimic::CreateMimic()
{
	return new Mimic();
}

void Mimic::storeInBundle(Bundle* bundle)
{
	Mob::storeInBundle(bundle);
	//bundle.put(ITEMS, items);
	//bundle.put(LEVEL, level);
}

void Mimic::restoreFromBundle(Bundle* bundle)
{
	Mob::restoreFromBundle(bundle);
	//items = new ArrayList<Item>((Collection< ? extends Item>) bundle.getCollection(ITEMS));
	//adjustStats(bundle.getInt(LEVEL));
}

int Mimic::damageRoll()
{
	return Random::NormalIntRange(HT / 10, HT / 4);
}

int Mimic::attackProc(Char* enemy, int damage)
{
	if (enemy == Dungeon::hero && Random::Int(3) == 0) {
		Gold gold(Random::Int(Dungeon::gold / 10, Dungeon::gold / 2));
		if (gold.Quantity() > 0) {
			Dungeon::gold -= gold.Quantity();
			Dungeon::level->drop(&gold, Dungeon::hero->pos)->sprite->drop();
		}
	}
	return Mob::attackProc(enemy, damage);
}

void Mimic::adjustStats(int level)
{
	this->level = level;

	HT = (3 + level) * 4;
	EXP = 2 + 2 * (level - 1) / 5;
	_defenseSkill = attackSkill(NULL) / 2;

	enemySeen = true;
}

void Mimic::die(const std::string& cause)
{
	Mob::die(cause);

	for (int i = 0; i < items.size();i++) {
		Item* item = items[i];
		Dungeon::level->drop(item, pos)->sprite->drop();
	}
}

Mimic* Mimic::spawnAt(int pos, std::vector<Item*>& items)
{
	Char* ch = Actor::findChar(pos);
	if (ch != NULL) {
		std::vector<int> candidates;
		for (int i = 0; i < 8; i++){
			int n = Level::NEIGHBOURS8[i];		
			int cell = pos + n;
			if ((Level::passable[cell] || Level::avoid[cell]) && Actor::findChar(cell) == NULL) {
				candidates.push_back(cell);
			}
		}
		if (candidates.size() > 0) {
			int newPos = RandomT<int>::element(candidates);
			//Actor::addDelayed(new Pushing(ch, ch.pos, newPos), -1);

			ch->pos = newPos;
			// FIXME
			if (dynamic_cast<Mob*>(ch)) {
				Dungeon::level->mobPress((Mob*)ch);
			}
			else {
				Dungeon::level->press(newPos, ch);
			}
		}
		else {
			return NULL;
		}
	}

	Mimic* m = new Mimic();	
	m->items = items;
	m->adjustStats(Dungeon::depth);
	m->HP = m->HT;
	m->pos = pos;
	m->state = m->HUNTING;
	GameScene::addMob(m, 1);

	m->sprite->turnTo(pos, Dungeon::hero->pos);

	if (Dungeon::visible[m->pos]) {
		CellEmitter::get(pos)->burst(Speck::factory(Speck::STAR), 10);
		//Sample.INSTANCE.play(Assets.SND_MIMIC);
	}

	return m;
}
