#pragma once

#include <string>

#include <set>
#include <map>
#include "typedefine.h"

#include "bundle.h"
#include "callback.h"

class Item;

class Badges{
public:
	class Badge{
	public:
		static std::map<std::string, Badge*> mapBadge;

		bool meta;
		std::string description;
		std::string name;
		int image;

		bool operator < (const Badge &c) const
		{
			return image < c.image;
		}

		bool operator == (const Badge &c) const
		{
			return image == c.image && description.compare(c.description) == 0 && meta == c.meta;
		}
	public:
		Badge(const Badge& other)
		{
			this->description = other.description;
			this->image = other.image;
			this->meta = other.meta;
			this->name = other.name;
		}
		Badge(const std::string& name, const std::string& description, int image) :Badge(name, description, image, false){}
		Badge(const std::string& name, const std::string& description, int image, bool meta)
		{
			this->description = description;
			this->image = image;
			this->meta = meta;
			this->name = name;

			mapBadge.insert(std::make_pair(name, this));
		}
		Badge(const std::string& name) :Badge(name, "", -1){}
	};

	static const Badge MONSTERS_SLAIN_1;
	static const Badge MONSTERS_SLAIN_2;
	static const Badge MONSTERS_SLAIN_3;
	static const Badge MONSTERS_SLAIN_4;
	static const Badge GOLD_COLLECTED_1;
	static const Badge GOLD_COLLECTED_2;
	static const Badge GOLD_COLLECTED_3;
	static const Badge GOLD_COLLECTED_4;
	static const Badge LEVEL_REACHED_1;
	static const Badge LEVEL_REACHED_2;
	static const Badge LEVEL_REACHED_3;
	static const Badge LEVEL_REACHED_4;
	static const Badge ALL_POTIONS_IDENTIFIED;
	static const Badge ALL_SCROLLS_IDENTIFIED;
	static const Badge ALL_RINGS_IDENTIFIED;
	static const Badge ALL_WANDS_IDENTIFIED;
	static const Badge ALL_ITEMS_IDENTIFIED;
	static const Badge BAG_BOUGHT_SEED_POUCH;
	static const Badge BAG_BOUGHT_SCROLL_HOLDER;
	static const Badge BAG_BOUGHT_WAND_HOLSTER;
	static const Badge ALL_BAGS_BOUGHT;
	static const Badge DEATH_FROM_FIRE;
	static const Badge DEATH_FROM_POISON;
	static const Badge DEATH_FROM_GAS;
	static const Badge DEATH_FROM_HUNGER;
	static const Badge DEATH_FROM_GLYPH;
	static const Badge DEATH_FROM_FALLING;
	static const Badge YASD;
	static const Badge BOSS_SLAIN_1_WARRIOR;
	static const Badge BOSS_SLAIN_1_MAGE;
	static const Badge BOSS_SLAIN_1_ROGUE;
	static const Badge BOSS_SLAIN_1_HUNTRESS;
	static const Badge BOSS_SLAIN_1;
	static const Badge BOSS_SLAIN_2;
	static const Badge BOSS_SLAIN_3;
	static const Badge BOSS_SLAIN_4;
	static const Badge BOSS_SLAIN_1_ALL_CLASSES;
	static const Badge BOSS_SLAIN_3_GLADIATOR;
	static const Badge BOSS_SLAIN_3_BERSERKER;
	static const Badge BOSS_SLAIN_3_WARLOCK;
	static const Badge BOSS_SLAIN_3_BATTLEMAGE;
	static const Badge BOSS_SLAIN_3_FREERUNNER;
	static const Badge BOSS_SLAIN_3_ASSASSIN;
	static const Badge BOSS_SLAIN_3_SNIPER;
	static const Badge BOSS_SLAIN_3_WARDEN;
	static const Badge BOSS_SLAIN_3_ALL_SUBCLASSES;
	static const Badge RING_OF_HAGGLER;
	static const Badge RING_OF_THORNS;
	static const Badge STRENGTH_ATTAINED_1;
	static const Badge STRENGTH_ATTAINED_2;
	static const Badge STRENGTH_ATTAINED_3;
	static const Badge STRENGTH_ATTAINED_4;
	static const Badge FOOD_EATEN_1;
	static const Badge FOOD_EATEN_2;
	static const Badge FOOD_EATEN_3;
	static const Badge FOOD_EATEN_4;
	static const Badge MASTERY_WARRIOR;
	static const Badge MASTERY_MAGE;
	static const Badge MASTERY_ROGUE;
	static const Badge MASTERY_HUNTRESS;
	static const Badge ITEM_LEVEL_1;
	static const Badge ITEM_LEVEL_2;
	static const Badge ITEM_LEVEL_3;
	static const Badge ITEM_LEVEL_4;
	static const Badge RARE_ALBINO;
	static const Badge RARE_BANDIT;
	static const Badge RARE_SHIELDED;
	static const Badge RARE_SENIOR;
	static const Badge RARE_ACIDIC;
	static const Badge RARE;
	static const Badge VICTORY_WARRIOR;
	static const Badge VICTORY_MAGE;
	static const Badge VICTORY_ROGUE;
	static const Badge VICTORY_HUNTRESS;
	static const Badge VICTORY;
	static const Badge VICTORY_ALL_CLASSES;
	static const Badge MASTERY_COMBO;
	static const Badge POTIONS_COOKED_1;
	static const Badge POTIONS_COOKED_2;
	static const Badge POTIONS_COOKED_3;
	static const Badge POTIONS_COOKED_4;
	static const Badge NO_MONSTERS_SLAIN;
	static const Badge GRIM_WEAPON;
	static const Badge PIRANHAS;
	static const Badge NIGHT_HUNTER;
	static const Badge GAMES_PLAYED_1;
	static const Badge GAMES_PLAYED_2;
	static const Badge GAMES_PLAYED_3;
	static const Badge GAMES_PLAYED_4;
	static const Badge HAPPY_END;
	static const Badge CHAMPION;
	static const Badge SUPPORTER;

public:
	static Callback* loadingListener;
private:
	static HashSet<Badge> global;
	static HashSet<Badge> local;

	static bool saveNeeded;

	
	static const std::string BADGES_FILE;
	static const std::string BADGES;

	static void restore(Bundle* bundle, std::set<Badge>& badges);
	static void store(Bundle* bundle, std::set<Badge>& badges);
	static void displayBadge(const Badge* badge);

	static void validateYASD();
public: 
	static void reset();
	static void loadGlobal();
	static void saveGlobal();
	static void loadLocal(Bundle* bundle);
	static void saveLocal(Bundle* bundle);

	static void validateMonstersSlain();
	static void validateGoldCollected();
	static void validateLevelReached();
	static void validateStrengthAttained();
	static void validateFoodEaten();
	static void validatePotionsCooked();
	static void validatePiranhasKilled();
	static void validateNoKilling();
	static void validateItemLevelAquired(Item* item);
	static void validateDeathFromFire();
	static void validateGrimWeapon();
	static void validateDeathFromPoison();
	static void validateDeathFromFalling();
	static void validateNightHunter();
	static void validateDeathFromGas();
	static void validateDeathFromGlyph();
	static void validateDeathFromHunger();
	static bool isUnlocked(Badge badge);
	static void validateVictory();
	static void validateChampion();
	static void validateHappyEnd();
};