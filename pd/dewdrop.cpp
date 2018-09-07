#include "dewdrop.h"
#include "simpleresource.h"
#include "speck.h"
#include "charsprite.h"
#include "hero.h"
#include "glog.h"
#include "belongings.h"
#include "dungeon.h"
#include "util.h"

const String Dewdrop::TXT_VALUE = "%+dHP";

Dewdrop::Dewdrop()
{
	name = "dewdrop";
	image = ItemSpriteSheet::DEWDROP;

	stackable = true;
}

boolean Dewdrop::doPickUp(Hero* hero)
{
	DewVial* vial = (DewVial*)hero->belongings->getItem("DewVial");

	if (hero->HP < hero->HT || vial == NULL || vial->isFull()) {

		int value = 1 + (Dungeon::depth - 1) / 5;
		if (hero->heroClass.type() == HeroClass::E_HUNTRESS) {
			value++;
		}

		int effect = std::min(hero->HT - hero->HP, value * quantity);
		if (effect > 0) {
			hero->HP += effect;
			hero->sprite->emitter()->burst(Speck::factory(Speck::HEALING), 1);
			hero->sprite->showStatus(CharSprite::POSITIVE, TXT_VALUE.c_str(), effect);
		}

	}
	else if (vial != NULL) {

		vial->collectDew(this);

	}

	//Sample.INSTANCE.play(Assets.SND_DEWDROP);
	hero->spendAndNext(TIME_TO_PICK_UP);

	return true;
}

const String DewVial::AC_DRINK = "DRINK";

const float DewVial::TIME_TO_DRINK = 1.0f;

const String DewVial::TXT_VALUE = "%+dHP";
const String DewVial::TXT_STATUS = "%d/%d";

const String DewVial::TXT_AUTO_DRINK = "The dew vial was emptied to heal your wounds.";
const String DewVial::TXT_COLLECTED = "You collected a dewdrop into your dew vial.";
const String DewVial::TXT_FULL = "Your dew vial is full!";
const String DewVial::TXT_EMPTY = "Your dew vial is empty!";
const String DewVial::VOLUME = "volume";

const double DewVial::NUM = 20;
const double DewVial::POW = std::log10(NUM);
ItemSprite::Glowing* DewVial::WHITE = new ItemSprite::Glowing(0xFFFFCC);

DewVial::DewVial()
{
	name = "dew vial";
	image = ItemSpriteSheet::VIAL;

	defaultAction = AC_DRINK;

	unique = true;
	volume = 0;
}

void DewVial::storeInBundle(Bundle* bundle)
{
	Item::storeInBundle(bundle);
	bundle->put(VOLUME, volume);
}

void DewVial::restoreFromBundle(Bundle* bundle)
{
	Item::restoreFromBundle(bundle);
	volume = bundle->getInt(VOLUME);
}

void DewVial::actions(Hero* hero, std::vector<std::string>& actions)
{
	Item::actions(hero, actions);
	if (volume > 0) {
		actions.push_back(AC_DRINK);
	}
}

void DewVial::execute(Hero* hero, std::string action)
{
	if (action.compare(AC_DRINK) == 0) {

		if (volume > 0) {

			int value = (int)std::ceil(std::pow(volume, POW) / NUM * hero->HT);
			int effect = std::min(hero->HT - hero->HP, value);
			if (effect > 0) {
				hero->HP += effect;
				hero->sprite->emitter()->burst(Speck::factory(Speck::HEALING), volume > 5 ? 2 : 1);
				hero->sprite->showStatus(CharSprite::POSITIVE, TXT_VALUE, effect);
			}

			volume = 0;

			hero->spend(TIME_TO_DRINK);
			hero->busy();

			//Sample.INSTANCE.play(Assets.SND_DRINK);
			hero->sprite->Operate(hero->pos);

			updateQuickslot();

		}
		else {
			GLog::w(TXT_EMPTY.c_str());
		}

	}
	else {

		Item::execute(hero, action);

	}
}

void DewVial::collectDew(Dewdrop* dew)
{
	GLog::i(TXT_COLLECTED.c_str());
	volume += dew->quantity;
	if (volume >= MAX_VOLUME) {
		volume = MAX_VOLUME;
		GLog::p(TXT_FULL.c_str());
	}

	updateQuickslot();
}

void DewVial::autoDrink(Hero* hero)
{
	DewVial* vial = (DewVial*)hero->belongings->getItem("DewVial");
	if (vial != NULL && vial->isFull()) {
		vial->execute(hero, "");
		//hero->sprite->emitter()->start(ShaftParticle.FACTORY, 0.2f, 3);

		GLog::w(TXT_AUTO_DRINK.c_str());
	}
}

String DewVial::status()
{
	return GameMath::format(TXT_STATUS.c_str(), volume, MAX_VOLUME);
}
