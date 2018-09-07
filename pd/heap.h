#pragma once
#include <vector>
#include "bundlable.h"
#include "itemsprite.h"
#include "typedefine.h"

class Item;
class ItemSprite;
class Hero;

class Heap :public Bundlable, public Object{
private:
	static const String TXT_MIMIC;
	static const int SEEDS_TO_POTION = 3;
	static const float FADE_TIME;

	static const String POS;
	static const String TYPE;
	static const String ITEMS;
public:
	enum Type {
		HEAP,
		FOR_SALE,
		CHEST,
		LOCKED_CHEST,
		CRYSTAL_CHEST,
		TOMB,
		SKELETON,
		MIMIC,
		HIDDEN
	};

	Type type;
	int pos;
	std::vector<Item* > items;
	ItemSprite* sprite;

	Heap();

	ItemSprite::Glowing* glowing();
	int image();
	int size() { return items.size(); }

	bool isEmpty() { return items.size() == 0; }
	Item* peek() { return items.size() > 0 ? items[0] : NULL; }

	void open(Hero* hero);
	void destroy();

	virtual void restoreFromBundle(Bundle* bundle);
	virtual void storeInBundle(Bundle* bundle);
	virtual std::string getClassName() { return "Heap"; }
	CLASSOBJECT(Heap);

	Item* pickUp();
	void drop(Item* item);
	void replace(Item* a, Item* b);

	void burn();
	void freeze();
	Item* transmute();
	static void burnFX(int pos);
	static void evaporateFX(int pos);
};