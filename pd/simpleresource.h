#pragma once
#include "ninepatch.h"
#include "typedefine.h"

class Chrome{
public:
	enum  Type {
		TOAST,
		TOAST_TR,
		WINDOW,
		BUTTON,
		TAG,
		SCROLL,
		TAB_SET,
		TAB_SELECTED,
		TAB_UNSELECTED
	};

	static NinePatch* get(Type type);
};

class ItemSpriteSheet {
public:
	// Placeholders
	static const int WEAPON = 5;
	static const int ARMOR = 6;
	static const int RING = 7;
	static const int SMTH = 127;

	// Keys
	static const int SKELETON_KEY = 8;
	static const int IRON_KEY = 9;
	static const int GOLDEN_KEY = 10;

	// Melee weapons
	static const int SHORT_SWORD = 2;
	static const int KNUCKLEDUSTER = 16;
	static const int QUARTERSTAFF = 17;
	static const int MACE = 18;
	static const int DAGGER = 19;
	static const int SWORD = 20;
	static const int LONG_SWORD = 21;
	static const int BATTLE_AXE = 22;
	static const int WAR_HAMMER = 23;
	static const int SPEAR = 29;
	static const int GLAIVE = 30;

	// Missiles
	static const int SHURIKEN = 15;
	static const int DART = 31;
	static const int BOOMERANG = 106;
	static const int TOMAHAWK = 107;
	static const int INCENDIARY_DART = 108;
	static const int CURARE_DART = 109;
	static const int JAVELIN = 110;

	// Armors
	static const int ARMOR_CLOTH = 24;
	static const int ARMOR_LEATHER = 25;
	static const int ARMOR_MAIL = 26;
	static const int ARMOR_SCALE = 27;
	static const int ARMOR_PLATE = 28;
	static const int ARMOR_ROGUE = 96;
	static const int ARMOR_WARRIOR = 97;
	static const int ARMOR_MAGE = 98;
	static const int ARMOR_HUNTRESS = 99;

	// Wands
	static const int WAND_MAGIC_MISSILE = 3;
	static const int WAND_HOLLY = 48;
	static const int WAND_YEW = 49;
	static const int WAND_EBONY = 50;
	static const int WAND_CHERRY = 51;
	static const int WAND_TEAK = 52;
	static const int WAND_ROWAN = 53;
	static const int WAND_WILLOW = 54;
	static const int WAND_MAHOGANY = 55;
	static const int WAND_BAMBOO = 68;
	static const int WAND_PURPLEHEART = 69;
	static const int WAND_OAK = 70;
	static const int WAND_BIRCH = 71;

	// Rings
	static const int RING_DIAMOND = 32;
	static const int RING_OPAL = 33;
	static const int RING_GARNET = 34;
	static const int RING_RUBY = 35;
	static const int RING_AMETHYST = 36;
	static const int RING_TOPAZ = 37;
	static const int RING_ONYX = 38;
	static const int RING_TOURMALINE = 39;
	static const int RING_EMERALD = 72;
	static const int RING_SAPPHIRE = 73;
	static const int RING_QUARTZ = 74;
	static const int RING_AGATE = 75;

	// Potions
	static const int POTION_TURQUOISE = 56;
	static const int POTION_CRIMSON = 57;
	static const int POTION_AZURE = 58;
	static const int POTION_JADE = 59;
	static const int POTION_GOLDEN = 60;
	static const int POTION_MAGENTA = 61;
	static const int POTION_CHARCOAL = 62;
	static const int POTION_IVORY = 63;
	static const int POTION_AMBER = 64;
	static const int POTION_BISTRE = 65;
	static const int POTION_INDIGO = 66;
	static const int POTION_SILVER = 67;

	// Scrolls
	static const int SCROLL_KAUNAN = 40;
	static const int SCROLL_SOWILO = 41;
	static const int SCROLL_LAGUZ = 42;
	static const int SCROLL_YNGVI = 43;
	static const int SCROLL_GYFU = 44;
	static const int SCROLL_RAIDO = 45;
	static const int SCROLL_ISAZ = 46;
	static const int SCROLL_MANNAZ = 47;
	static const int SCROLL_NAUDIZ = 76;
	static const int SCROLL_BERKANAN = 77;
	static const int SCROLL_ODAL = 78;
	static const int SCROLL_TIWAZ = 79;
	static const int SCROLL_WIPE_OUT = 117;

	// Seeds
	static const int SEED_FIREBLOOM = 88;
	static const int SEED_ICECAP = 89;
	static const int SEED_SORROWMOSS = 90;
	static const int SEED_DREAMWEED = 91;
	static const int SEED_SUNGRASS = 92;
	static const int SEED_EARTHROOT = 93;
	static const int SEED_FADELEAF = 94;
	static const int SEED_ROTBERRY = 95;

	// Quest items
	static const int ROSE = 100;
	static const int PICKAXE = 101;
	static const int ORE = 102;
	static const int SKULL = 103;
	static const int PHANTOM = 118;
	static const int DUST = 121;
	static const int TOKEN = 122;

	// Heaps
	static const int BONES = 0;
	static const int CHEST = 11;
	static const int LOCKED_CHEST = 12;
	static const int TOMB = 13;
	static const int CRYSTAL_CHEST = 105;
	static const int HIDDEN = 119;

	// Food
	static const int RATION = 4;
	static const int PASTY = 112;
	static const int MEAT = 113;
	static const int STEAK = 114;
	static const int OVERPRICED = 115;
	static const int CARPACCIO = 116;

	// Bags
	static const int POUCH = 83;
	static const int HOLDER = 104;
	static const int HOLSTER = 111;
	static const int KEYRING = 126;

	// UNUSED
	static const int SPARK_UPGRADE = 117;
	static const int SPARK_ENCHANT = 118;

	// Misc
	static const int ANKH = 1;
	static const int GOLD = 14;
	static const int STYLUS = 80;
	static const int DEWDROP = 81;
	static const int MASTERY = 82;
	static const int TORCH = 84;
	static const int BEACON = 85;
	static const int KIT = 86;
	static const int AMULET = 87;
	static const int VIAL = 120;
	static const int WEIGHT = 123;
	static const int BOMB = 124;
	static const int HONEYPOT = 125;
};

class Image;
class Effects {
public:
	enum  Type {
		RIPPLE,
		LIGHTNING,
		WOUND,
		RAY
	};

	static Image* get(Type type);
};

class ResultDescriptions {

	// Mobs 
public:
	static const std::string MOB ;
	static const std::string BOSS ;

	// Items
	static const std::string WAND ;
	static const std::string GLYPH ;
	static const std::string BOMB ;

	// Dungeon features
	static const std::string TRAP ;

	// Debuffs & blobs
	static const std::string BURNING ;
	static const std::string HUNGER ;
	static const std::string POISON ;
	static const std::string GAS ;
	static const std::string BLEEDING ;
	static const std::string OOZE ;
	static const std::string FALL ;

	static const std::string WIN ;
};

class BlobFire;
class Freezing{
public:
	static boolean affect(int cell, BlobFire* fire);
};

class Sign{
private:
	static const String TXT_DEAD_END;
	static const String _TIPS[];
	static const String TXT_BURN;

public:
	static void read(int pos);
};