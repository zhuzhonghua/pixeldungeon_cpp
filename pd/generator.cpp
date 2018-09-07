#include "stdafx.h"
#include "generator.h"
#include "util.h"
#include "armor.h"
#include "dungeon.h"
#include "weapon.h"
#include "gold.h"
#include "scroll.h"
#include "potion.h"
#include "wand.h"
#include "weapon.h"
#include "ring.h"
#include "food.h"
#include "plant.h"
#include "items.h"
#include "hero.h"

std::map<Generator::Category::Type, Generator::Category*> Generator::Category::cats;

bool Generator::Category::initFlag = false;

std::map<Generator::Category::Type, float> Generator::categoryProbs;

Generator::Category Generator::WEAPON(Generator::Category::WEAPON);
Generator::Category Generator::ARMOR(Generator::Category::ARMOR);
Generator::Category Generator::POTION(Generator::Category::POTION);
Generator::Category Generator::SCROLL(Generator::Category::SCROLL);
Generator::Category Generator::WAND(Generator::Category::WAND);
Generator::Category Generator::RING(Generator::Category::RING);
Generator::Category Generator::SEED(Generator::Category::SEED);
Generator::Category Generator::FOOD(Generator::Category::FOOD);
Generator::Category Generator::GOLD(Generator::Category::GOLD);
Generator::Category Generator::MISC(Generator::Category::MISC);

Item* Generator::random()
{
	init();

	return random(RandomT<Category::Type>::chances(categoryProbs));
}

Item* Generator::random(Category::Type cat)
{
	init();

	categoryProbs.insert(std::make_pair(cat, categoryProbs[cat] / 2));

	switch (cat) {
	case Category::ARMOR:
		return randomArmor();
	case Category::WEAPON:
		return randomWeapon();
	default:
		Category* cate = Category::cats[cat];
		return ((Item*)cate->classes[Random::chances(cate->probs)]())->random();
	}
}

Armor* Generator::randomArmor()
{
	init();

	int curStr = Hero::STARTING_STR + Dungeon::potionOfStrength;

	Category cat = ARMOR;

	Armor* a1 = (Armor*)cat.classes[Random::chances(cat.probs)]();
	Armor* a2 = (Armor*)cat.classes[Random::chances(cat.probs)]();

	a1->random();
	a2->random();

	return std::abs(curStr - a1->STR) < std::abs(curStr - a2->STR) ? a1 : a2;
}

Weapon* Generator::randomWeapon()
{
	init();

	int curStr = Hero::STARTING_STR + Dungeon::potionOfStrength;

	Category cat = WEAPON;

	Weapon* w1 = (Weapon*)cat.classes[Random::chances(cat.probs)]();
	Weapon* w2 = (Weapon*)cat.classes[Random::chances(cat.probs)]();

	w1->random();
	w2->random();

	return std::abs(curStr - w1->STR) < std::abs(curStr - w2->STR) ? w1 : w2;
}

void Generator::reset()
{
	for (std::map<Category::Type, Category*>::iterator itr = Category::cats.begin();
		itr != Category::cats.end(); itr++){
		categoryProbs.insert(std::make_pair(itr->first, itr->second->prob));
	}
}

void Generator::init()
{
	if (Category::initFlag == false){
		Category::initFlag = true;

		Category::CreateItem goldClasses[] = { Gold::Create };
		GOLD.classes = std::vector<Category::CreateItem>(goldClasses, goldClasses + sizeof(goldClasses) / sizeof(Category::CreateItem));
		float goldProbs[] = { 1 };
		GOLD.probs = std::vector<float>(goldProbs, goldProbs+sizeof(goldProbs)/sizeof(float));

		Category::CreateItem scrollClasses[] = { 
			ScrollOfIdentify::Create, 
			ScrollOfTeleportation::Create,
			ScrollOfRemoveCurse::Create,
			ScrollOfRecharging::Create,
			ScrollOfMagicMapping::Create,
			ScrollOfChallenge::Create,
			ScrollOfTerror::Create,
			ScrollOfLullaby::Create,
			ScrollOfPsionicBlast::Create,
			ScrollOfMirrorImage::Create,
			ScrollOfUpgrade::Create,
			ScrollOfEnchantment::Create
		};
		SCROLL.classes = std::vector<Category::CreateItem>(scrollClasses, scrollClasses + sizeof(scrollClasses) / sizeof(Category::CreateItem));
		float scrollProbs[] = { 30, 10, 15, 10, 15, 12, 8, 8, 4, 6, 0, 1 };
		SCROLL.probs = std::vector<float>(scrollProbs, scrollProbs + sizeof(scrollProbs) / sizeof(float));

		Category::CreateItem potionClasses[] = {
			PotionOfHealing::Create,
			PotionOfExperience::Create,
			PotionOfToxicGas::Create,
			PotionOfParalyticGas::Create,
			PotionOfLiquidFlame::Create,
			PotionOfLevitation::Create,
			PotionOfStrength::Create,
			PotionOfMindVision::Create,
			PotionOfPurity::Create,
			PotionOfInvisibility::Create,
			PotionOfMight::Create,
			PotionOfFrost::Create,
		};
		POTION.classes = std::vector<Category::CreateItem>(potionClasses, potionClasses + sizeof(potionClasses) / sizeof(Category::CreateItem));
		float potionProbs[] = { 45, 4, 15, 10, 15, 10, 0, 20, 12, 10, 0, 10 };
		POTION.probs = std::vector<float>(potionProbs, potionProbs + sizeof(potionProbs) / sizeof(float));

		Category::CreateItem wandClasses[] = {
			WandOfTeleportation::Create,
			WandOfSlowness::Create,
			WandOfFirebolt::Create,
			WandOfRegrowth::Create,
			WandOfPoison::Create,
			WandOfBlink::Create,
			WandOfLightning::Create,
			WandOfAmok::Create,
			WandOfReach::Create,
			WandOfFlock::Create,
			WandOfMagicMissile::Create,
			WandOfDisintegration::Create,
			WandOfAvalanche::Create
		};
		WAND.classes = std::vector<Category::CreateItem>(wandClasses, wandClasses + sizeof(wandClasses) / sizeof(Category::CreateItem));;
		float wandProbs[] = { 10, 10, 15, 6, 10, 11, 15, 10, 6, 10, 0, 5, 5 };
		WAND.probs = std::vector<float>(wandProbs, wandProbs + sizeof(wandProbs) / sizeof(float));

		Category::CreateItem weaponClasses[] = {
			Dagger::Create,
			Knuckles::Create,
			Quarterstaff::Create,
			Spear::Create,
			Mace::Create,
			Sword::Create,
			Longsword::Create,
			BattleAxe::Create,
			WarHammer::Create,
			Glaive::Create,
			ShortSword::Create,
			Dart::Create,
			Javelin::Create,
			IncendiaryDart::Create,
			CurareDart::Create,
			Shuriken::Create,
			Boomerang::Create,
			Tamahawk::Create
		};
		WEAPON.classes = std::vector<Category::CreateItem>(weaponClasses, weaponClasses + sizeof(weaponClasses) / sizeof(Category::CreateItem));;
		float weaponProbs[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1 };
		WEAPON.probs = std::vector<float>(weaponProbs, weaponProbs + sizeof(weaponProbs) / sizeof(float));

		Category::CreateItem armorClasses[] = {
			ClothArmor::Create,
			LeatherArmor::Create,
			MailArmor::Create,
			ScaleArmor::Create,
			PlateArmor::Create
		};
		ARMOR.classes = std::vector<Category::CreateItem>(armorClasses, armorClasses + sizeof(armorClasses) / sizeof(Category::CreateItem));;
		float armornProbs[] = { 1, 1, 1, 1, 1 };
		ARMOR.probs = std::vector<float>(armornProbs, armornProbs + sizeof(armornProbs) / sizeof(float));

		Category::CreateItem foodClasses[] = {
			Food::Create,
			Pasty::Create,
			MysteryMeat::Create
		};
		FOOD.classes = std::vector<Category::CreateItem>(foodClasses, foodClasses + sizeof(foodClasses) / sizeof(Category::CreateItem));;
		float foodProbs[] = { 4, 1, 0 };
		FOOD.probs = std::vector<float>(foodProbs, foodProbs + sizeof(foodProbs) / sizeof(float));

		Category::CreateItem ringClasses[] = {
			RingOfMending::Create,
			RingOfDetection::Create,
			RingOfShadows::Create,
			RingOfPower::Create,
			RingOfHerbalism::Create,
			RingOfAccuracy::Create,
			RingOfEvasion::Create,
			RingOfSatiety::Create,
			RingOfHaste::Create,
			RingOfElements::Create,
			RingOfHaggler::Create,
			RingOfThorns::Create
		};
		RING.classes = std::vector<Category::CreateItem>(ringClasses, ringClasses + sizeof(ringClasses) / sizeof(Category::CreateItem));
		float ringProbs[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 };
		RING.probs = std::vector<float>(ringProbs, ringProbs + sizeof(ringProbs) / sizeof(float));
		
		Category::CreateItem plantClasses[] = {
			Firebloom::Seed::Create,
			Icecap::Seed::Create,
			Sorrowmoss::Seed::Create,
			Dreamweed::Seed::Create,
			Sungrass::Seed::Create,
			Earthroot::Seed::Create,
			Fadeleaf::Seed::Create,
			Rotberry::Seed::Create
		};
		SEED.classes = std::vector<Category::CreateItem>(plantClasses, plantClasses + sizeof(plantClasses) / sizeof(Category::CreateItem));
		float plantProbs[] = { 1, 1, 1, 1, 1, 1, 1, 0 };
		SEED.probs = std::vector<float>(plantProbs, plantProbs + sizeof(plantProbs) / sizeof(float));
		
		Category::CreateItem miscClasses[] = {
			Bomb::Create,
			Honeypot::Create
		};
		MISC.classes = std::vector<Category::CreateItem>(miscClasses, miscClasses + sizeof(miscClasses) / sizeof(Category::CreateItem));
		float miscProbs[] = { 2, 1 };
		MISC.probs = std::vector<float>(miscProbs, miscProbs + sizeof(miscProbs) / sizeof(float));
	}
}
