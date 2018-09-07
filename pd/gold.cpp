#include "gold.h"
#include "simpleresource.h"
#include "dungeon.h"
#include "statistics.h"
#include "gamescene.h"
#include "hero.h"
#include "charsprite.h"
#include "util.h"

const std::string Gold::TXT_COLLECT = "Collect gold coins to spend them later in a shop.";
const std::string Gold::TXT_INFO = "A pile of %d gold coins. " + TXT_COLLECT;
const std::string Gold::TXT_INFO_1 = "One gold coin. " + TXT_COLLECT;
const std::string Gold::TXT_VALUE = "%+d";

Gold::Gold()
:Gold(1)
{
	
}

Gold::Gold(int value)
{
	this->quantity = value;

	name = "gold";
	image = ItemSpriteSheet::GOLD;
	stackable = true;
}

void Gold::actions(Hero* hero, std::vector<std::string>& actions)
{
	// Nothing
}

bool Gold::doPickUp(Hero* hero)
{
	Dungeon::gold += quantity;
	Statistics::goldCollected += quantity;
	Badges::validateGoldCollected();

	GameScene::pickUp(this);
	hero->sprite->showStatus(CharSprite::NEUTRAL, TXT_VALUE, quantity);
	hero->spendAndNext(TIME_TO_PICK_UP);

	//Sample.INSTANCE.play(Assets.SND_GOLD, 1, 1, Random.Float(0.9f, 1.1f));

	return true;
}

std::string Gold::info()
{
	switch (quantity) 
	{
	case 0:
		return TXT_COLLECT;
	case 1:
		return TXT_INFO_1;
	default:
		return GameMath::format(TXT_INFO.c_str(), quantity);
	}
}

Item* Gold::random()
{
	quantity = Random::Int(20 + Dungeon::depth * 10, 40 + Dungeon::depth * 20);
	return this;
}

void Gold::storeInBundle(Bundle* bundle)
{
	Item::storeInBundle(bundle);
	bundle->put(VALUE, quantity);
}

void Gold::restoreFromBundle(Bundle* bundle)
{
	Item::restoreFromBundle(bundle);
	quantity = bundle->getInt(VALUE);
}

const std::string Gold::VALUE = "value";