#include "badge.h"
#include "util.h"
#include "bundle.h"
#include "statistics.h"
#include "dungeon.h"
#include "bpt.h"
#include "glog.h"
#include "pixelscene.h"
#include "hero.h"

#include <sstream>

std::map<std::string, Badges::Badge*> Badges::Badge::mapBadge;

const Badges::Badge Badges::MONSTERS_SLAIN_1("MONSTERS_SLAIN_1", BPT::getText("lang.badge_MONSTERS_SLAIN_1_desc"), 0);
const Badges::Badge Badges::MONSTERS_SLAIN_2("MONSTERS_SLAIN_2", BPT::getText("lang.badge_MONSTERS_SLAIN_2_desc"), 1);
const Badges::Badge Badges::MONSTERS_SLAIN_3("MONSTERS_SLAIN_3", BPT::getText("lang.badge_MONSTERS_SLAIN_3_desc"), 2);
const Badges::Badge Badges::MONSTERS_SLAIN_4("MONSTERS_SLAIN_4", BPT::getText("lang.badge_MONSTERS_SLAIN_4_desc"), 3);
const Badges::Badge Badges::GOLD_COLLECTED_1("GOLD_COLLECTED_1", BPT::getText("lang.badge_GOLD_COLLECTED_1_desc"), 4);
const Badges::Badge Badges::GOLD_COLLECTED_2("GOLD_COLLECTED_2", BPT::getText("lang.badge_GOLD_COLLECTED_2_desc"), 5);
const Badges::Badge Badges::GOLD_COLLECTED_3("GOLD_COLLECTED_3", BPT::getText("lang.badge_GOLD_COLLECTED_3_desc"), 6);
const Badges::Badge Badges::GOLD_COLLECTED_4("GOLD_COLLECTED_4", BPT::getText("lang.badge_GOLD_COLLECTED_4_desc"), 7);
const Badges::Badge Badges::LEVEL_REACHED_1("LEVEL_REACHED_1", BPT::getText("lang.badge_LEVEL_REACHED_1_desc"), 8);
const Badges::Badge Badges::LEVEL_REACHED_2("LEVEL_REACHED_2", BPT::getText("lang.badge_LEVEL_REACHED_2_desc"), 9);
const Badges::Badge Badges::LEVEL_REACHED_3("LEVEL_REACHED_3", BPT::getText("lang.badge_LEVEL_REACHED_3_desc"), 10);
const Badges::Badge Badges::LEVEL_REACHED_4("LEVEL_REACHED_4", BPT::getText("lang.badge_LEVEL_REACHED_4_desc"), 11);
const Badges::Badge Badges::ALL_POTIONS_IDENTIFIED("ALL_POTIONS_IDENTIFIED", BPT::getText("lang.badge_ALL_POTIONS_IDENTIFIED_desc"), 16);
const Badges::Badge Badges::ALL_SCROLLS_IDENTIFIED("ALL_SCROLLS_IDENTIFIED", BPT::getText("lang.badge_ALL_SCROLLS_IDENTIFIED_desc"), 17);
const Badges::Badge Badges::ALL_RINGS_IDENTIFIED("ALL_RINGS_IDENTIFIED", BPT::getText("lang.badge_ALL_RINGS_IDENTIFIED_desc"), 18);
const Badges::Badge Badges::ALL_WANDS_IDENTIFIED("ALL_WANDS_IDENTIFIED", BPT::getText("lang.badge_ALL_WANDS_IDENTIFIED_desc"), 19);
const Badges::Badge Badges::ALL_ITEMS_IDENTIFIED("ALL_ITEMS_IDENTIFIED", BPT::getText("lang.badge_ALL_ITEMS_IDENTIFIED_desc"), 35, true);
const Badges::Badge Badges::BAG_BOUGHT_SEED_POUCH("BAG_BOUGHT_SEED_POUCH");
const Badges::Badge Badges::BAG_BOUGHT_SCROLL_HOLDER("BAG_BOUGHT_SCROLL_HOLDER");
const Badges::Badge Badges::BAG_BOUGHT_WAND_HOLSTER("BAG_BOUGHT_WAND_HOLSTER");
const Badges::Badge Badges::ALL_BAGS_BOUGHT("ALL_BAGS_BOUGHT", BPT::getText("lang.badge_ALL_BAGS_BOUGHT_desc"), 23);
const Badges::Badge Badges::DEATH_FROM_FIRE("DEATH_FROM_FIRE", BPT::getText("lang.badge_DEATH_FROM_FIRE_desc"), 24);
const Badges::Badge Badges::DEATH_FROM_POISON("DEATH_FROM_POISON", BPT::getText("lang.badge_DEATH_FROM_POISON_desc"), 25);
const Badges::Badge Badges::DEATH_FROM_GAS("DEATH_FROM_GAS", BPT::getText("lang.badge_DEATH_FROM_GAS_desc"), 26);
const Badges::Badge Badges::DEATH_FROM_HUNGER("DEATH_FROM_HUNGER", BPT::getText("lang.badge_DEATH_FROM_HUNGER_desc"), 27);
const Badges::Badge Badges::DEATH_FROM_GLYPH("DEATH_FROM_GLYPH", BPT::getText("lang.badge_DEATH_FROM_GLYPH_desc"), 57);
const Badges::Badge Badges::DEATH_FROM_FALLING("DEATH_FROM_FALLING", BPT::getText("lang.badge_DEATH_FROM_FALLING_desc"), 59);
const Badges::Badge Badges::YASD("YASD", BPT::getText("lang.badge_YASD_desc"), 34, true);
const Badges::Badge Badges::BOSS_SLAIN_1_WARRIOR("BOSS_SLAIN_1_WARRIOR");
const Badges::Badge Badges::BOSS_SLAIN_1_MAGE("BOSS_SLAIN_1_MAGE");
const Badges::Badge Badges::BOSS_SLAIN_1_ROGUE("BOSS_SLAIN_1_ROGUE");
const Badges::Badge Badges::BOSS_SLAIN_1_HUNTRESS("BOSS_SLAIN_1_HUNTRESS");
const Badges::Badge Badges::BOSS_SLAIN_1("BOSS_SLAIN_1", BPT::getText("lang.badge_BOSS_SLAIN_1_desc"), 12);
const Badges::Badge Badges::BOSS_SLAIN_2("BOSS_SLAIN_2", BPT::getText("lang.badge_BOSS_SLAIN_2_desc"), 13);
const Badges::Badge Badges::BOSS_SLAIN_3("BOSS_SLAIN_3", BPT::getText("lang.badge_BOSS_SLAIN_3_desc"), 14);
const Badges::Badge Badges::BOSS_SLAIN_4("BOSS_SLAIN_4", BPT::getText("lang.badge_BOSS_SLAIN_4_desc"), 15);
const Badges::Badge Badges::BOSS_SLAIN_1_ALL_CLASSES("BOSS_SLAIN_1_ALL_CLASSES", BPT::getText("lang.badge_BOSS_SLAIN_1_ALL_CLASSES_desc"), 32, true);
const Badges::Badge Badges::BOSS_SLAIN_3_GLADIATOR("BOSS_SLAIN_3_GLADIATOR");
const Badges::Badge Badges::BOSS_SLAIN_3_BERSERKER("BOSS_SLAIN_3_BERSERKER");
const Badges::Badge Badges::BOSS_SLAIN_3_WARLOCK("BOSS_SLAIN_3_WARLOCK");
const Badges::Badge Badges::BOSS_SLAIN_3_BATTLEMAGE("BOSS_SLAIN_3_BATTLEMAGE");
const Badges::Badge Badges::BOSS_SLAIN_3_FREERUNNER("BOSS_SLAIN_3_FREERUNNER");
const Badges::Badge Badges::BOSS_SLAIN_3_ASSASSIN("BOSS_SLAIN_3_ASSASSIN");
const Badges::Badge Badges::BOSS_SLAIN_3_SNIPER("BOSS_SLAIN_3_SNIPER");
const Badges::Badge Badges::BOSS_SLAIN_3_WARDEN("BOSS_SLAIN_3_WARDEN");
const Badges::Badge Badges::BOSS_SLAIN_3_ALL_SUBCLASSES("BOSS_SLAIN_3_ALL_SUBCLASSES", BPT::getText("lang.badge_BOSS_SLAIN_3_ALL_SUBCLASSES_desc"), 33, true);
const Badges::Badge Badges::RING_OF_HAGGLER("RING_OF_HAGGLER", BPT::getText("lang.badge_RING_OF_HAGGLER_desc"), 20);
const Badges::Badge Badges::RING_OF_THORNS("RING_OF_THORNS", BPT::getText("lang.badge_RING_OF_THORNS_desc"), 21);
const Badges::Badge Badges::STRENGTH_ATTAINED_1("STRENGTH_ATTAINED_1", BPT::getText("lang.badge_STRENGTH_ATTAINED_1_desc"), 40);
const Badges::Badge Badges::STRENGTH_ATTAINED_2("STRENGTH_ATTAINED_2", BPT::getText("lang.badge_STRENGTH_ATTAINED_2_desc"), 41);
const Badges::Badge Badges::STRENGTH_ATTAINED_3("STRENGTH_ATTAINED_3", BPT::getText("lang.badge_STRENGTH_ATTAINED_3_desc"), 42);
const Badges::Badge Badges::STRENGTH_ATTAINED_4("STRENGTH_ATTAINED_4", BPT::getText("lang.badge_STRENGTH_ATTAINED_4_desc"), 43);
const Badges::Badge Badges::FOOD_EATEN_1("FOOD_EATEN_1", BPT::getText("lang.badge_FOOD_EATEN_1_desc"), 44);
const Badges::Badge Badges::FOOD_EATEN_2("FOOD_EATEN_2", BPT::getText("lang.badge_FOOD_EATEN_2_desc"), 45);
const Badges::Badge Badges::FOOD_EATEN_3("FOOD_EATEN_3", BPT::getText("lang.badge_FOOD_EATEN_3_desc"), 46);
const Badges::Badge Badges::FOOD_EATEN_4("FOOD_EATEN_4", BPT::getText("lang.badge_FOOD_EATEN_4_desc"), 47);
const Badges::Badge Badges::MASTERY_WARRIOR("MASTERY_WARRIOR");
const Badges::Badge Badges::MASTERY_MAGE("MASTERY_MAGE");
const Badges::Badge Badges::MASTERY_ROGUE("MASTERY_ROGUE");
const Badges::Badge Badges::MASTERY_HUNTRESS("MASTERY_HUNTRESS");
const Badges::Badge Badges::ITEM_LEVEL_1("ITEM_LEVEL_1", BPT::getText("lang.badge_ITEM_LEVEL_1_desc"), 48);
const Badges::Badge Badges::ITEM_LEVEL_2("ITEM_LEVEL_2", BPT::getText("lang.badge_ITEM_LEVEL_2_desc"), 49);
const Badges::Badge Badges::ITEM_LEVEL_3("ITEM_LEVEL_3", BPT::getText("lang.badge_ITEM_LEVEL_3_desc"), 50);
const Badges::Badge Badges::ITEM_LEVEL_4("ITEM_LEVEL_4", BPT::getText("lang.badge_ITEM_LEVEL_4_desc"), 51);
const Badges::Badge Badges::RARE_ALBINO("RARE_ALBINO");
const Badges::Badge Badges::RARE_BANDIT("RARE_BANDIT");
const Badges::Badge Badges::RARE_SHIELDED("RARE_SHIELDED");
const Badges::Badge Badges::RARE_SENIOR("RARE_SENIOR");
const Badges::Badge Badges::RARE_ACIDIC("RARE_ACIDIC");
const Badges::Badge Badges::RARE("RARE", BPT::getText("lang.badge_RARE_desc"), 37, true);
const Badges::Badge Badges::VICTORY_WARRIOR("VICTORY_WARRIOR");
const Badges::Badge Badges::VICTORY_MAGE("VICTORY_MAGE");
const Badges::Badge Badges::VICTORY_ROGUE("VICTORY_ROGUE");
const Badges::Badge Badges::VICTORY_HUNTRESS("VICTORY_HUNTRESS");
const Badges::Badge Badges::VICTORY("VICTORY", BPT::getText("lang.badge_VICTORY_desc"), 22);
const Badges::Badge Badges::VICTORY_ALL_CLASSES("VICTORY_ALL_CLASSES", BPT::getText("lang.badge_VICTORY_ALL_CLASSES_desc"), 36, true);
const Badges::Badge Badges::MASTERY_COMBO("MASTERY_COMBO", BPT::getText("lang.badge_MASTERY_COMBO_desc"), 56);
const Badges::Badge Badges::POTIONS_COOKED_1("POTIONS_COOKED_1", BPT::getText("lang.badge_POTIONS_COOKED_1_desc"), 52);
const Badges::Badge Badges::POTIONS_COOKED_2("POTIONS_COOKED_2", BPT::getText("lang.badge_POTIONS_COOKED_2_desc"), 53);
const Badges::Badge Badges::POTIONS_COOKED_3("POTIONS_COOKED_3", BPT::getText("lang.badge_POTIONS_COOKED_3_desc"), 54);
const Badges::Badge Badges::POTIONS_COOKED_4("POTIONS_COOKED_4", BPT::getText("lang.badge_POTIONS_COOKED_4_desc"), 55);
const Badges::Badge Badges::NO_MONSTERS_SLAIN("NO_MONSTERS_SLAIN", BPT::getText("lang.badge_NO_MONSTERS_SLAIN_desc"), 28);
const Badges::Badge Badges::GRIM_WEAPON("GRIM_WEAPON", BPT::getText("lang.badge_GRIM_WEAPON_desc"), 29);
const Badges::Badge Badges::PIRANHAS("PIRANHAS", BPT::getText("lang.badge_PIRANHAS_desc"), 30);
const Badges::Badge Badges::NIGHT_HUNTER("NIGHT_HUNTER", BPT::getText("lang.badge_NIGHT_HUNTER_desc"), 58);
const Badges::Badge Badges::GAMES_PLAYED_1("GAMES_PLAYED_1", BPT::getText("lang.badge_GAMES_PLAYED_1_desc"), 60, true);
const Badges::Badge Badges::GAMES_PLAYED_2("GAMES_PLAYED_2", BPT::getText("lang.badge_GAMES_PLAYED_2_desc"), 61, true);
const Badges::Badge Badges::GAMES_PLAYED_3("GAMES_PLAYED_3", BPT::getText("lang.badge_GAMES_PLAYED_3_desc"), 62, true);
const Badges::Badge Badges::GAMES_PLAYED_4("GAMES_PLAYED_4", BPT::getText("lang.badge_GAMES_PLAYED_4_desc"), 63, true);
const Badges::Badge Badges::HAPPY_END("HAPPY_END", BPT::getText("lang.badge_HAPPY_END_desc"), 38);
const Badges::Badge Badges::CHAMPION("CHAMPION", BPT::getText("lang.badge_CHAMPION_desc"), 39, true);
const Badges::Badge Badges::SUPPORTER("SUPPORTER", BPT::getText("lang.badge_SUPPORTER_desc"), 31, true);

HashSet<Badges::Badge> Badges::global;
HashSet<Badges::Badge> Badges::local;

bool Badges::saveNeeded = false;

Callback* Badges::loadingListener = NULL;

const std::string Badges::BADGES_FILE = "badges.dat";
const std::string Badges::BADGES = "badges";

void Badges::reset()
{
	local.clear();
	loadGlobal();
}

void Badges::restore(Bundle* bundle, std::set<Badges::Badge>& badges)
{
	std::list<std::string> names;
	bundle->getStringArray(BADGES, names);

	for (std::list<std::string>::iterator itr = names.begin();
		itr != names.end(); itr++)
	{
		badges.insert(*(Badge::mapBadge.find(*itr)->second));
	}
}

void Badges::store(Bundle* bundle, std::set<Badges::Badge>& badges)
{
	std::vector<std::string> names;
	names.resize(badges.size());
	
	int count = 0;
	for (std::set<Badges::Badge>::iterator itr = badges.begin();
		itr != badges.end(); itr++)
	{
		names[count++] = itr->name;
	}

	bundle->put(BADGES, names);
}

void Badges::displayBadge(const Badges::Badge* badge)
{
	if (badge == NULL) 
	{
		return;
	}

	if (global.find(*badge) != global.end()) 
	{
		if (!badge->meta) 
		{
			GLog::h("Badge endorsed: %s", badge->description);
		}

	}
	else 
	{

		global.insert(*badge);
		saveNeeded = true;

		if (badge->meta) 
		{
			GLog::h("New super badge: %s", badge->description);
		}
		else 
		{
			GLog::h("New badge: %s", badge->description);
		}
		PixelScene::showBadge(badge);
	}
}

void Badges::validateYASD()
{
	if (global.contains(Badges::DEATH_FROM_FIRE) &&
		global.contains(Badges::DEATH_FROM_POISON) &&
		global.contains(Badges::DEATH_FROM_GAS) &&
		global.contains(Badges::DEATH_FROM_HUNGER)) {

		Badge badge = Badges::YASD;
		local.add(badge);
		displayBadge(&badge);
	}
}

void Badges::loadGlobal()
{
	if (global.size() <= 0) 
	{
		std::stringstream ss;
		if (IOManager::readFileToBuffer(BADGES_FILE, ss, true))
		{
			Bundle* bund = Bundle::read(ss);
			restore(bund, global);
			delete bund;
		}
	}
}

void Badges::saveGlobal()
{
	Bundle bundle;
	if (saveNeeded)
	{
		store(&bundle, global);

		std::stringstream ss;
		Bundle::write(&bundle, ss);

		IOManager::writeFile(BADGES_FILE, ss.str());

		saveNeeded = false;
	}
}

void Badges::loadLocal(Bundle* bundle)
{
	restore(bundle, local);
}

void Badges::saveLocal(Bundle* bundle)
{
	store(bundle, local);
}

void Badges::validateMonstersSlain()
{
	const Badge* badge = NULL;

	if (local.find(Badges::MONSTERS_SLAIN_1)==local.end() && Statistics::enemiesSlain >= 10) 
	{
		badge = &Badges::MONSTERS_SLAIN_1;
		local.insert(*badge);
	}
	if (local.find(Badges::MONSTERS_SLAIN_2) == local.end() && Statistics::enemiesSlain >= 50) 
	{
		badge = &Badges::MONSTERS_SLAIN_2;
		local.insert(*badge);
	}
	if (local.find(Badges::MONSTERS_SLAIN_3) == local.end() && Statistics::enemiesSlain >= 150) 
	{
		badge = &Badges::MONSTERS_SLAIN_3;
		local.insert(*badge);
	}
	if (local.find(Badges::MONSTERS_SLAIN_4) == local.end() && Statistics::enemiesSlain >= 250) 
	{
		badge = &Badges::MONSTERS_SLAIN_4;
		local.insert(*badge);
	}

	displayBadge(badge);
}

void Badges::validateGoldCollected()
{
	const Badge* badge = NULL;

	if (local.find(Badges::GOLD_COLLECTED_1) == local.end() && Statistics::goldCollected >= 100) 
	{
		badge = &Badges::GOLD_COLLECTED_1;
		local.insert(*badge);
	}
	if (local.find(Badges::GOLD_COLLECTED_2) == local.end() && Statistics::goldCollected >= 500) 
	{
		badge = &Badges::GOLD_COLLECTED_2;
		local.insert(*badge);
	}
	if (local.find(Badges::GOLD_COLLECTED_3) == local.end() && Statistics::goldCollected >= 2500) 
	{
		badge = &Badges::GOLD_COLLECTED_3;
		local.insert(*badge);
	}
	if (local.find(Badges::GOLD_COLLECTED_4) == local.end() && Statistics::goldCollected >= 7500) 
	{
		badge = &Badges::GOLD_COLLECTED_4;
		local.insert(*badge);
	}

	displayBadge(badge);
}

void Badges::validateLevelReached()
{
	const Badge* badge = NULL;

	if (local.find(Badges::LEVEL_REACHED_1) == local.end() && Dungeon::hero->lvl >= 6) 
	{
		badge = &Badges::LEVEL_REACHED_1;
		local.insert(*badge);
	}
	if (local.find(Badges::LEVEL_REACHED_2) == local.end() && Dungeon::hero->lvl >= 12)
	{
		badge = &Badges::LEVEL_REACHED_2;
		local.insert(*badge);
	}
	if (local.find(Badges::LEVEL_REACHED_3) == local.end() && Dungeon::hero->lvl >= 18)
	{
		badge = &Badges::LEVEL_REACHED_3;
		local.insert(*badge);
	}
	if (local.find(Badges::LEVEL_REACHED_4) == local.end() && Dungeon::hero->lvl >= 24)
	{
		badge = &Badges::LEVEL_REACHED_4;
		local.insert(*badge);
	}

	displayBadge(badge);
}

void Badges::validateStrengthAttained()
{
	const Badge* badge = NULL;

	if (local.find(Badges::STRENGTH_ATTAINED_1) == local.end() && Dungeon::hero->STR >= 13) {
		badge = &Badges::STRENGTH_ATTAINED_1;
		local.insert(*badge);
	}
	if (local.find(Badges::STRENGTH_ATTAINED_2) == local.end() && Dungeon::hero->STR >= 15) {
		badge = &Badges::STRENGTH_ATTAINED_2;
		local.insert(*badge);
	}
	if (local.find(Badges::STRENGTH_ATTAINED_3) == local.end() && Dungeon::hero->STR >= 17) {
		badge = &Badges::STRENGTH_ATTAINED_3;
		local.insert(*badge);
	}
	if (local.find(Badges::STRENGTH_ATTAINED_4) == local.end() && Dungeon::hero->STR >= 19) {
		badge = &Badges::STRENGTH_ATTAINED_4;
		local.insert(*badge);
	}

	displayBadge(badge);
}

void Badges::validateFoodEaten()
{
	const Badge* badge = NULL;

	if (local.find(Badges::FOOD_EATEN_1) == local.end() && Statistics::foodEaten >= 10) 
	{
		badge = &Badges::FOOD_EATEN_1;
		local.insert(*badge);
	}
	if (local.find(Badges::FOOD_EATEN_2) == local.end() && Statistics::foodEaten >= 20) 
	{
		badge = &Badges::FOOD_EATEN_2;
		local.insert(*badge);
	}
	if (local.find(Badges::FOOD_EATEN_3) == local.end() && Statistics::foodEaten >= 30) 
	{
		badge = &Badges::FOOD_EATEN_3;
		local.insert(*badge);
	}
	if (local.find(Badges::FOOD_EATEN_4) == local.end() && Statistics::foodEaten >= 40) 
	{
		badge = &Badges::FOOD_EATEN_4;
		local.insert(*badge);
	}

	displayBadge(badge);
}

void Badges::validatePotionsCooked()
{
	const Badge* badge = NULL;

	if (local.find(Badges::POTIONS_COOKED_1) == local.end() && Statistics::potionsCooked >= 3) 
	{
		badge = &Badges::POTIONS_COOKED_1;
		local.insert(*badge);
	}
	if (local.find(Badges::POTIONS_COOKED_2) == local.end() && Statistics::potionsCooked >= 6) 
	{
		badge = &Badges::POTIONS_COOKED_2;
		local.insert(*badge);
	}
	if (local.find(Badges::POTIONS_COOKED_3) == local.end() && Statistics::potionsCooked >= 9) 
	{
		badge = &Badges::POTIONS_COOKED_3;
		local.insert(*badge);
	}
	if (local.find(Badges::POTIONS_COOKED_4) == local.end() && Statistics::potionsCooked >= 12) 
	{
		badge = &Badges::POTIONS_COOKED_4;
		local.insert(*badge);
	}

	displayBadge(badge);
}

void Badges::validatePiranhasKilled()
{
	const Badge* badge = NULL;

	if (local.find(Badges::PIRANHAS) == local.end() && Statistics::piranhasKilled >= 6) 
	{
		badge = &Badges::PIRANHAS;
		local.insert(*badge);
	}

	displayBadge(badge);
}

void Badges::validateNoKilling()
{
	if (local.find(Badges::NO_MONSTERS_SLAIN) == local.end() && Statistics::completedWithNoKilling) 
	{
		Badge badge = Badges::NO_MONSTERS_SLAIN;
		local.insert(badge);
		displayBadge(&badge);
	}
}

void Badges::validateItemLevelAquired(Item* item)
{
	// This method should be called:
	// 1) When an item gets obtained (Item.collect)
	// 2) When an item gets upgraded (ScrollOfUpgrade, ScrollOfWeaponUpgrade, ShortSword, WandOfMagicMissile)
	// 3) When an item gets identified
	if (!item->levelKnown) {
		return;
	}

	const Badge* badge = NULL;

	if (!local.contains(Badges::ITEM_LEVEL_1) && item->Level() >= 3) {
		badge = &Badges::ITEM_LEVEL_1;
		local.add(*badge);
	}
	if (!local.contains(Badges::ITEM_LEVEL_2) && item->Level() >= 6) {
		badge = &Badges::ITEM_LEVEL_2;
		local.add(*badge);
	}
	if (!local.contains(Badges::ITEM_LEVEL_3) && item->Level() >= 9) {
		badge = &Badges::ITEM_LEVEL_3;
		local.add(*badge);
	}
	if (!local.contains(Badges::ITEM_LEVEL_4) && item->Level() >= 12) {
		badge = &Badges::ITEM_LEVEL_4;
		local.add(*badge);
	}

	displayBadge(badge);
}

void Badges::validateDeathFromFire()
{
	Badge badge = Badges::DEATH_FROM_FIRE;
	local.add(badge);
	displayBadge(&badge);

	validateYASD();
}

void Badges::validateGrimWeapon()
{
	if (!local.contains(Badges::GRIM_WEAPON)) {
		Badge badge = Badges::GRIM_WEAPON;
		local.add(badge);
		displayBadge(&badge);
	}
}

void Badges::validateDeathFromPoison()
{
	Badge badge = Badges::DEATH_FROM_POISON;
	local.add(badge);
	displayBadge(&badge);

	validateYASD();
}

void Badges::validateDeathFromFalling()
{
	Badge badge = Badges::DEATH_FROM_FALLING;
	local.insert(badge);
	displayBadge(&badge);
}

void Badges::validateNightHunter()
{
	if (!local.contains(Badges::NIGHT_HUNTER) && Statistics::nightHunt >= 15) {
		Badge badge = Badges::NIGHT_HUNTER;
		local.add(badge);
		displayBadge(&badge);
	}
}

bool Badges::isUnlocked(Badges::Badge badge)
{
	return global.find(badge) != global.end();
}

void Badges::validateDeathFromGas()
{
	Badge badge = Badges::DEATH_FROM_GAS;
	local.add(badge);
	displayBadge(&badge);

	validateYASD();
}

void Badges::validateDeathFromGlyph()
{
	Badge badge = Badges::DEATH_FROM_GLYPH;
	local.add(badge);
	displayBadge(&badge);
}

void Badges::validateDeathFromHunger()
{
	Badge badge = Badges::DEATH_FROM_HUNGER;
	local.add(badge);
	displayBadge(&badge);

	validateYASD();
}

void Badges::validateVictory()
{
	Badge badge = Badges::VICTORY;
	displayBadge(&badge);

	switch (Dungeon::hero->heroClass.type()) {
	case HeroClass::E_WARRIOR:
		badge = Badges::VICTORY_WARRIOR;
		break;
	case HeroClass::E_MAGE:
		badge = Badges::VICTORY_MAGE;
		break;
	case HeroClass::E_ROGUE:
		badge = Badges::VICTORY_ROGUE;
		break;
	case HeroClass::E_HUNTRESS:
		badge = Badges::VICTORY_HUNTRESS;
		break;
	}
	local.add(badge);
	if (!global.contains(badge)) {
		global.add(badge);
		saveNeeded = true;
	}

	if (global.contains(Badges::VICTORY_WARRIOR) &&
		global.contains(Badges::VICTORY_MAGE) &&
		global.contains(Badges::VICTORY_ROGUE) &&
		global.contains(Badges::VICTORY_HUNTRESS)) {

		badge = Badges::VICTORY_ALL_CLASSES;
		displayBadge(&badge);
	}
}

void Badges::validateChampion()
{
	displayBadge(&Badges::CHAMPION);
}

void Badges::validateHappyEnd()
{
	displayBadge(&Badges::HAPPY_END);
}
