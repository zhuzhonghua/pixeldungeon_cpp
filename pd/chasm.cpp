#include "chasm.h"
#include "gamescene.h"
#include "wndoptions.h"
#include "hero.h"
#include "dungeon.h"
#include "interlevelscene.h"
#include "regularlevel.h"
#include "charsprite.h"
#include "game.h"
#include "badge.h"
#include "glog.h"
#include "simpleresource.h"
#include "mob.h"
#include "mobsprite.h"
#include "herodoom.h"

const std::string Chasm::TXT_CHASM = "Chasm";
const std::string Chasm::TXT_YES = "Yes, I know what I'm doing";
const std::string Chasm::TXT_NO = "No, I changed my mind";
const std::string Chasm::TXT_JUMP =
"Do you really want to jump into the chasm? You can probably die.";

bool Chasm::jumpConfirmed = false;

namespace{
	class WndOptionsNew :public WndOptions{
	public:
		Hero* hero;
		WndOptionsNew(Hero*h, const std::string& title, const std::string& message, const std::vector<std::string>& options)
			:WndOptions(title, message, options),
			hero(h){}
	protected:
		virtual void onSelect(int index)
		{
			if (index == 0) {
				Chasm::jumpConfirmed = true;
				hero->resume();
			}
		}
	};
}
void Chasm::heroJump(Hero* hero)
{
	std::vector<std::string> options;
	options.push_back(TXT_YES);
	options.push_back(TXT_NO);
	GameScene::show(new WndOptionsNew(hero, TXT_CHASM, TXT_JUMP, options));
}

void Chasm::heroFall(int pos)
{
	jumpConfirmed = false;

	//Sample.INSTANCE.play(Assets.SND_FALLING);

	if (Dungeon::hero->isAlive()) {
		Dungeon::hero->interrupt();
		InterlevelScene::mode = InterlevelScene::Mode::FALL;
		if (dynamic_cast<RegularLevel*>(Dungeon::level)) {
			Room* room = ((RegularLevel*)Dungeon::level)->room(pos);
			InterlevelScene::fallIntoPit = room != NULL && room->type == Room::Type::WEAK_FLOOR;
		}
		else {
			InterlevelScene::fallIntoPit = false;
		}
		Game::switchScene(new InterlevelScene());
	}
	else {
		Dungeon::hero->sprite->visible = false;
	}
}

namespace{
	class HeroDoomNew :public HeroDoom, public Object{
	public:
		virtual void onDeath()
		{
			Badges::validateDeathFromFalling();

			Dungeon::fail(GameMath::format(ResultDescriptions::FALL.c_str(), Dungeon::depth));
			GLog::n("You fell to death...");
		}
		virtual std::string getObject() { return "Hero::Doom"; }
	};
}
void Chasm::heroLand()
{
	Hero* hero = Dungeon::hero;

	hero->sprite->burst(hero->sprite->blood(), 10);
	Camera::mainCamera->shake(4, 0.2f);

	Buff::prolong(hero, "Cripple", Cripple::DURATION);

	HeroDoomNew src;
	hero->damage(Random::IntRange(hero->HT / 3, hero->HT / 2), "");
}

void Chasm::mobFall(Mob* mob)
{
	// TODO: Check Memory Leak
	mob->destroy();
	((MobSprite*)mob->sprite)->fall();
}
