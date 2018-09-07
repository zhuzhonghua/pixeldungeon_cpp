#include "bag.h"
#include "simpleresource.h"
#include "plant.h"

const std::string Bag::AC_OPEN = "OPEN";
const std::string Bag::ITEMS = "inventory";

Bag::Bag()
{
	image = 11;

	defaultAction = AC_OPEN;

	owner = NULL;
	size = 1;
}

void Bag::storeInBundle(Bundle* bundle)
{
	Item::storeInBundle(bundle);
	//bundle.put(ITEMS, items);
}

void Bag::restoreFromBundle(Bundle* bundle)
{
	Item::restoreFromBundle(bundle);
	//for (Bundlable item : bundle.getCollection(ITEMS)) {
	//	((Item)item).collect(this);
	//};
}

bool Bag::contains(Item* item)
{
	for (ArrayList<Item*>::iterator itr = items.begin();
		itr != items.end(); itr++)
	{
		Item* i = *itr;
		if (i == item) {
			return true;
		}
		else if (dynamic_cast<Bag*>(i) && ((Bag*)i)->contains(item)) {
			return true;
		}
	}
	return false;
}

Item* Bag::Next()
{
	if (nextBag != NULL && nextBag->HasNext()){
		return nextBag->Next();
	}
	else{
		nextBag = NULL;

		itr++;

		Item* itm = *itr;
		if (dynamic_cast<Bag*>(itm)){
			nextBag = (Bag*)itm;
		}
		return itm;
	}
}

void Bag::Begin()
{
	nextBag = NULL;
	itr = items.begin();
}

bool Bag::HasNext()
{
	if (nextBag != NULL){
		return nextBag->HasNext() || itr != items.end();
	}
	else{
		return itr != items.end();
	}
}


//REFLECTITEM(Bag);

SeedPouch::SeedPouch()
{
	name = "seed pouch";
	image = ItemSpriteSheet::POUCH;

	size = 8;
}

boolean SeedPouch::grab(Item* item)
{
	return dynamic_cast<Plant::Seed*>(item) != NULL;
}


REFLECTITEM(SeedPouch);