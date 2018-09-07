#include "food.h"
#include "simpleresource.h"
#include "hero.h"
#include "belongings.h"
#include "glog.h"
#include "speck.h"
#include "charsprite.h"
#include "statistics.h"
#include "glog.h"
#include "util.h"

const float Food::TIME_TO_EAT = 3.0f;

const std::string Food::AC_EAT = "EAT";

Food::Food()
{
	stackable = true;
	name = "ration of food";
	image = ItemSpriteSheet::RATION;

	//energy = Hunger.HUNGRY;
	message = "That food tasted delicious!";
}

void Food::actions(Hero* hero, std::vector<std::string>& actions)
{
	Item::actions(hero, actions);
	actions.push_back(AC_EAT);
}

void Food::execute(Hero* hero, const std::string& action)
{
	if (action.compare(AC_EAT) == 0) 
	{
		detach(hero->belongings->backpack);

		//((Hunger)hero.buff(Hunger.class)).satisfy(energy);
		GLog::i(message.c_str());

		switch (hero->heroClass.type()) 
		{
		case HeroClass::E_WARRIOR:
			if (hero->HP < hero->HT) 
			{
				hero->HP = std::min(hero->HP + 5, hero->HT);
				hero->sprite->emitter()->burst(Speck::factory(Speck::HEALING), 1);
			}
			break;
		case HeroClass::E_MAGE:
			//hero->belongings->charge(false);
			//ScrollOfRecharging.charge(hero);
			break;
		case HeroClass::E_ROGUE:
		case HeroClass::E_HUNTRESS:
			break;
		}

		//hero->sprite->operate(hero->pos);
		hero->busy();
		//SpellSprite.show(hero, SpellSprite.FOOD);
		//Sample.INSTANCE.play(Assets.SND_EAT);

		//hero->spend(TIME_TO_EAT);

		Statistics::foodEaten++;
		Badges::validateFoodEaten();
	}
	else 
	{
		Item::execute(hero, action);
	}
}

MysteryMeat::MysteryMeat()
{
	name = "mystery meat";
	image = ItemSpriteSheet::MEAT;
	//energy = Hunger.STARVING - Hunger.HUNGRY;
	message = "That food tasted... strange.";
}

void MysteryMeat::execute(Hero* hero, const std::string& action)
{
	Food::execute(hero, action);

	if (action.compare(AC_EAT) == 0) {

		switch (Random::Int(5)) {
		case 0:
			GLog::w("Oh it's hot!");
			//Buff::affect(hero, Burning.class).reignite(hero);
			break;
		case 1:
			GLog::w("You can't feel your legs!");
			//Buff::prolong(hero, Roots.class, Paralysis.duration(hero));
			break;
		case 2:
			GLog::w("You are not feeling well.");
			//Buff::affect(hero, Poison.class).set(Poison.durationFactor(hero) * hero.HT / 5);
			break;
		case 3:
			GLog::w("You are stuffed.");
			//Buff::prolong(hero, Slow.class, Slow.duration(hero));
			break;
		}
	}
}

ChargrilledMeat::ChargrilledMeat()
{
	name = "chargrilled meat";
	image = ItemSpriteSheet::STEAK;
	//energy = Hunger.STARVING - Hunger.HUNGRY;
}

Food* ChargrilledMeat::cook(MysteryMeat* ingredient)
{
	ChargrilledMeat* result = new ChargrilledMeat();
	result->quantity = ingredient->Quantity();
	return result;
}

FrozenCarpaccio::FrozenCarpaccio()
{
	name = "frozen carpaccio";
	image = ItemSpriteSheet::CARPACCIO;
	//energy = Hunger.STARVING - Hunger.HUNGRY;
}

void FrozenCarpaccio::execute(Hero* hero, const std::string& action)
{
	Food::execute(hero, action);

	if (action.compare(AC_EAT) == 0) {

		switch (Random::Int(5)) {
		case 0:
			GLog::i("You see your hands turn invisible!");
			//Buff::affect(hero, Invisibility.class, Invisibility.DURATION);
			break;
		case 1:
			GLog::i("You feel your skin hardens!");
			//Buff::affect(hero, Barkskin.class).level(hero.HT / 4);
			break;
		case 2:
			GLog::i("Refreshing!");
			//Buff::detach(hero, Poison.class);
			//Buff::detach(hero, Cripple.class);
			//Buff::detach(hero, Weakness.class);
			//Buff::detach(hero, Bleeding.class);
			break;
		case 3:
			GLog::i("You feel better!");
			if (hero->HP < hero->HT) {
				hero->HP = std::min(hero->HP + hero->HT / 4, hero->HT);
				hero->sprite->emitter()->burst(Speck::factory(Speck::HEALING), 1);
			}
			break;
		}
	}
}

Food* FrozenCarpaccio::cook(MysteryMeat* ingredient)
{
	FrozenCarpaccio* result = new FrozenCarpaccio();
	result->quantity = ingredient->Quantity();
	return result;
}

Pasty::Pasty()
{
	name = "pasty";
	image = ItemSpriteSheet::PASTY;
	//energy = Hunger.STARVING;
}
