#include "items.h"
#include "simpleresource.h"
#include "level.h"
#include "dungeon.h"
#include "gamescene.h"
#include "glog.h"
#include "hero.h"
#include "charsprite.h"
#include "belongings.h"
#include "splash.h"
#include "statistics.h"
#include "badge.h"
#include "amuletscene.h"
#include "game.h"
#include "cellemitter.h"
#include "pixelparticle.h"

Bomb::Bomb()
{
	name = "bomb";
	image = ItemSpriteSheet::BOMB;
	defaultAction = AC_THROW;
	stackable = true;
}

Item* Bomb::random()
{
	quantity = Random::IntRange(1, 3);
	return this;
}

void Bomb::onThrow(int cell)
{
	if (Level::pit[cell]) {
		Item::onThrow(cell);
	}
	else {
		//Sample.INSTANCE.play(Assets.SND_BLAST, 2);

		if (Dungeon::visible[cell]) {
			CellEmitter::center(cell)->burst(BlastParticle::factory(), 30);
		}

		boolean terrainAffected = false;
		for (int i = 0; i < 9;i++){
			int n = Level::NEIGHBOURS9[i];
		//for (int n : Level.NEIGHBOURS9) {
			int c = cell + n;
			if (c >= 0 && c < Level::LENGTH) {
				if (Dungeon::visible[c]) {
					//CellEmitter::get(c)->burst(SmokeParticle.FACTORY, 4);
				}

				if (Level::flamable[c]) {
					Dungeon::level->destroy(c);
					GameScene::updateMap(c);
					terrainAffected = true;
				}

				Char* ch = Actor::findChar(c);
				if (ch != NULL) {
					int dmg = Random::Int(1 + Dungeon::depth, 10 + Dungeon::depth * 2) - Random::Int(ch->dr());
					if (dmg > 0) {
						ch->damage(dmg, this->getClassName());
						if (ch->isAlive()) {
							//Buff.prolong(ch, Paralysis.class, 2);
						}
						else if (ch == Dungeon::hero) {
							//Dungeon.fail(Utils.format(ResultDescriptions.BOMB, Dungeon.depth));
							GLog::n("You killed yourself with a bomb...");
						}
					}
				}
			}
		}

		if (terrainAffected) {
			Dungeon::observe();
		}
	}
}

const String Honeypot::AC_SHATTER = "SHATTER";

Honeypot::Honeypot()
{
	name = "honeypot";
	image = ItemSpriteSheet::HONEYPOT;
	defaultAction = AC_THROW;
	stackable = true;
}

void Honeypot::actions(Hero* hero, std::vector<std::string>& actions)
{
	Item::actions(hero, actions);
	actions.push_back(AC_SHATTER);
}

void Honeypot::execute(Hero* hero, std::string action)
{
	if (action.compare(AC_SHATTER) == 0) {

		hero->sprite->Zap(hero->pos);
		shatter(hero->pos);

		detach(hero->belongings->backpack);
		hero->spendAndNext(TIME_TO_THROW);

	}
	else {
		Item::execute(hero, action);
	}
}

void Honeypot::shatter(int pos)
{
	//Sample.INSTANCE.play(Assets.SND_SHATTER);

	if (Dungeon::visible[pos]) {
		Splash::at(pos, 0xffd500, 5);
	}

	int newPos = pos;
	if (Actor::findChar(pos) != NULL) {
		std::vector<int> candidates;
		std::vector<bool> passable = Level::passable;

		for (int i = 0; i < 4;i++){
			int n = Level::NEIGHBOURS4[i];
		//for (int n : Level.NEIGHBOURS4) {
			int c = pos + n;
			if (passable[c] && Actor::findChar(c) == NULL) {
				candidates.push_back(c);
			}
		}

		newPos = candidates.size() > 0 ? RandomT<int>::element(candidates) : -1;
	}

	if (newPos != -1) {
		//Bee bee = new Bee();
		//bee.spawn(Dungeon.depth);
		//bee.HP = bee.HT;
		//bee.pos = newPos;
		//
		//GameScene.add(bee);
		//Actor.addDelayed(new Pushing(bee, pos, newPos), -1);
		//
		//bee.sprite.alpha(0);
		//bee.sprite.parent.add(new AlphaTweener(bee.sprite, 1, 0.15f));
		//
		//Sample.INSTANCE.play(Assets.SND_BEE);
	}
}

const String Amulet::AC_END = "END THE GAME";

Amulet::Amulet()
{
	name = "Amulet of Yendor";
	image = ItemSpriteSheet::AMULET;

	unique = true;
}

void Amulet::actions(Hero* hero, std::vector<std::string>& actions)
{
	Item::actions(hero, actions);
	actions.push_back(AC_END);
}

void Amulet::execute(Hero* hero, std::string action)
{
	if (action == AC_END) {

		showAmuletScene(false);

	}
	else {

		Item::execute(hero, action);

	}
}

bool Amulet::doPickUp(Hero* hero)
{
	if (Item::doPickUp(hero)) {

		if (!Statistics::amuletObtained) {
			Statistics::amuletObtained = true;
			Badges::validateVictory();

			showAmuletScene(true);
		}

		return true;
	}
	else {
		return false;
	}
}

void Amulet::showAmuletScene(boolean showText)
{
	Dungeon::saveAll();
	AmuletScene::noText = !showText;
	Game::switchScene(new AmuletScene);
}
