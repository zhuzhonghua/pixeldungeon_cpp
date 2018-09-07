#pragma once

#include <map>

class Item;
class Armor;
class Weapon;

class Generator{
public:
	class Category{
	public:
		enum Type{
			WEAPON = 15,
			ARMOR = 10,
			POTION = 50,
			SCROLL = 40,
			WAND = 4,
			RING = 2,
			SEED = 5,
			FOOD = 0,
			GOLD = 50,
			MISC = 5,
		};

		typedef Item* (*CreateItem)();

		std::vector<CreateItem> classes;
		Type type;
		std::vector<float> probs;
		float prob;

		static std::map<Type, Category*> cats;

		static bool initFlag;

		Category(Type t) :type(t),prob(t)
		{
			cats.insert(std::make_pair(t, this));
		}
	};

	static Category WEAPON;
	static Category ARMOR;
	static Category POTION;
	static Category SCROLL;
	static Category WAND;
	static Category RING;
	static Category SEED;
	static Category FOOD;
	static Category GOLD;
	static Category MISC;
public:
	static Item* random();
	static Item* random(Category::Type cat);
	static Armor* randomArmor();
	static Weapon* randomWeapon();
	static void reset();
private:
	static void init();
	static std::map<Category::Type, float> categoryProbs;
};