#include "belongings.h"
#include "bag.h"
#include "util.h"
#include "key.h"
#include "kindofweapon.h"
#include "ring.h"
#include "dungeon.h"
#include "armor.h"
#include "hero.h"
#include "char.h"

const std::string Belongings::WEAPON = "weapon";
const std::string Belongings::ARMOR = "armor";
const std::string Belongings::RING1 = "ring1";
const std::string Belongings::RING2 = "ring2";

Belongings::Belongings(Hero* owner)
{
	this->owner = owner;

	backpack = new Bag();
	backpack->name = "backpack";
	backpack->size = BACKPACK_SIZE;
	backpack->owner = owner;

	weapon = NULL;
	armor = NULL;
	ring1 = NULL;
	ring2 = NULL;
}

Item* Belongings::getItem(const std::string& itemClass)
{
	for (int i = 0; i < _items.size(); i++){
		if (_items[i]->getClassName() == itemClass){
			return _items[i];
		}
	}

	return NULL;
}

Item* Belongings::randomUnequipped()
{
	return RandomT<Item*>::element(backpack->items);
}

void Belongings::resurrect(int depth)
{
	ArrayList<Item*> tempItems = backpack->items;
	for (ArrayList<Item*>::iterator itr = tempItems.begin();
		itr != tempItems.end(); itr++)
	{
		Item* item = *itr;
		if (dynamic_cast<Key*>(item)){
			if (((Key*)item)->depth == depth) {
				item->detachAll(backpack);
			}
		}
		else if (item->unique) {
			// Keep unique items
		}
		else if (!item->isEquipped(owner)) {
			item->detachAll(backpack);
		}
	}

	if (weapon != NULL) {
		weapon->cursed = false;
		weapon->activate(owner);
	}

	if (armor != NULL) {
		armor->cursed = false;
	}

	if (ring1 != NULL) {
		ring1->cursed = false;
		ring1->activate(owner);
	}
	if (ring2 != NULL) {
		ring2->cursed = false;
		ring2->activate(owner);
	}
}

Key* Belongings::getKey(const std::string& kind, int depth)
{
	backpack->Begin();
	while (backpack->HasNext()){
		Item* item = backpack->Next();
		if (item->getClassName().compare(kind) == 0 && ((Key*)item)->depth == depth){
			return (Key*)item;
		}
	}

	return NULL;
}

void Belongings::countIronKeys()
{
	IronKey::curDepthQuantity = 0;

	backpack->Begin();
	while (backpack->HasNext())
	{
		Item* item = backpack->Next();
		if (dynamic_cast<IronKey*>(item) && ((IronKey*)item)->depth == Dungeon::depth)
		{
			IronKey::curDepthQuantity++;
		}
	}
}

void Belongings::identify()
{
	backpack->Begin();
	while (backpack->HasNext())
	{
		Item* item = backpack->Next();
		item->identify();
	}
}
