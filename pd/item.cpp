#include "item.h"
#include "gamescene.h"
#include "util.h"
#include "bpt.h"
#include "dungeon.h"
#include "level.h"
#include "heap.h"
#include "charsprite.h"
#include "speck.h"
#include "bag.h"
#include "quickslot.h"
#include "hero.h"

const char* Item::TXT_PACK_FULL = "lang.item_packfull";

const char* Item::TXT_BROKEN = "lang.item_broken";
const char* Item::TXT_GONNA_BREAK = "lang.item_goingbroken";

const char* Item::TXT_TO_STRING = "%s";
const char* Item::TXT_TO_STRING_X = "%s x%d";
const char* Item::TXT_TO_STRING_LVL = "%s%+d";
const char* Item::TXT_TO_STRING_LVL_X = "%s%+d x%d";

const float Item::DURABILITY_WARNING_LEVEL = 1 / 6.0f;

const float Item::TIME_TO_THROW = 1.0f;
const float Item::TIME_TO_PICK_UP = 1.0f;
const float Item::TIME_TO_DROP = 0.5f;

const std::string Item::AC_DROP = "lang.item_drop";
const std::string Item::AC_THROW = "lang.item_throw";

std::map<std::string, FactoryItem*> FactoryItem::facs;

void Item::onThrow(int cell)
{
	Heap* heap = Dungeon::level->drop(this, cell);
	if (!heap->isEmpty()) {
		heap->sprite->drop(cell);
	}
}

Hero* Item::curUser;
Item* Item::curItem;

namespace{
	class CellSelectorListenerNew :public CellSelector::Listener{
	public:
		virtual void onSelect(int cell)
		{
			if (cell != -1)
			{
				Item::curItem->cast(Item::curUser, cell);
			}
		}
		virtual std::string prompt()
		{
			return "Choose direction of throw";
		}
	};
}

CellSelector::Listener* Item::thrower = new CellSelectorListenerNew();

Item::Item()
{
	quantity = 1;
}

void Item::actions(Hero* hero, std::vector<std::string>& actions)
{
	actions.push_back(BPT::getText(AC_DROP));
	actions.push_back(BPT::getText(AC_THROW));
}

bool Item::doPickUp(Hero* hero)
{
	//if (collect(hero.belongings.backpack)) 
	//{
	//
	//	GameScene.pickUp(this);
	//	//Sample.INSTANCE.play(Assets.SND_ITEM);
	//	hero.spendAndNext(TIME_TO_PICK_UP);
	//	return true;
	//}
	//else 
	{
		return false;
	}
}

void Item::doDrop(Hero* hero)
{
	//hero.spendAndNext(TIME_TO_DROP);
	//Dungeon.level.drop(detachAll(hero.belongings.backpack), hero.pos).sprite.drop(hero.pos);
}

void Item::doThrow(Hero* hero)
{
	GameScene::selectCell(thrower);
}

void Item::evoke(Hero* hero)
{
	hero->sprite->emitter()->burst(Speck::factory(Speck::EVOKE), 5);
}

std::string Item::toString()
{
	if (levelKnown && level != 0) 
	{
		if (quantity > 1) 
		{
			return GameMath::format(TXT_TO_STRING_LVL_X, Name(), level, quantity);
		}
		else 
		{
			return GameMath::format(TXT_TO_STRING_LVL, Name(), level);
		}
	}
	else 
	{
		if (quantity > 1) 
		{
			return GameMath::format(TXT_TO_STRING_X, Name(), quantity);
		}
		else 
		{
			return GameMath::format(TXT_TO_STRING, Name());
		}
	}
}

std::string Item::Name()
{
	return name;
}

int Item::Image()
{
	return image;
}

Item* Item::detach(Bag* container)
{
	//if (quantity <= 0) 
	//{
	//	return NULL;
	//}
	//else
	//if (quantity == 1) 
	//{
	//	return detachAll(container);
	//}
	//else 
	//{
	//	quantity--;
	//	updateQuickslot();
	//
	//	try {
	//		Item detached = getClass().newInstance();
	//		detached.onDetach();
	//		return detached;
	//	}
	//	catch (Exception e) {
	//		return null;
	//	}
	//}
	return NULL;
}

Item* Item::detachAll(Bag* container)
{
	for (ArrayList<Item*>::iterator itr = container->items.begin();
		itr != container->items.end(); itr++)
	{
		Item* item = *itr;
		if (item == this) {
			container->items.remove(this);
			item->onDetach();
			QuickSlot::refresh();
			return this;
		}
		else if (dynamic_cast<Bag*>(item)) {
			Bag* bag = (Bag*)item;
			if (bag->contains(this)) {
				return detachAll(bag);
			}
		}
	}

	return this;
}

void Item::storeInBundle(Bundle* bundle)
{
	//bundle.put(QUANTITY, quantity);
	//bundle.put(LEVEL, level);
	//bundle.put(LEVEL_KNOWN, levelKnown);
	//bundle.put(CURSED, cursed);
	//bundle.put(CURSED_KNOWN, cursedKnown);
	//if (isUpgradable()) {
	//	bundle.put(DURABILITY, durability);
	//}
	//QuickSlot.save(bundle, this);
}

void Item::restoreFromBundle(Bundle* bundle)
{
	//quantity = bundle.getInt(QUANTITY);
	//levelKnown = bundle.getBoolean(LEVEL_KNOWN);
	//cursedKnown = bundle.getBoolean(CURSED_KNOWN);
	//
	//int level = bundle.getInt(LEVEL);
	//if (level > 0) {
	//	upgrade(level);
	//}
	//else if (level < 0) {
	//	degrade(-level);
	//}
	//
	//cursed = bundle.getBoolean(CURSED);
	//
	//if (isUpgradable()) {
	//	durability = bundle.getInt(DURABILITY);
	//}
	//
	//QuickSlot.restore(bundle, this);
}

int Item::effectiveLevel()
{
	return 0;// isBroken() ? 0 : level;
}

void Item::execute(Hero* hero, std::string straction)
{
	curUser = hero;
	curItem = this;

	if (straction.compare(AC_DROP) == 0)
	{
		doDrop(hero);
	}
	else if (straction.compare(AC_THROW) == 0)
	{
		doThrow(hero);
	}
}

void Item::cast(Hero* user, int dst)
{
	//final int cell = Ballistica.cast(user.pos, dst, false, true);
	//user.sprite.zap(cell);
	//user.busy();
	//
	//Sample.INSTANCE.play(Assets.SND_MISS, 0.6f, 0.6f, 1.5f);
	//
	//Char enemy = Actor.findChar(cell);
	//QuickSlot.target(this, enemy);
	//
	//// FIXME!!!
	//float delay = TIME_TO_THROW;
	//if (this instanceof MissileWeapon) {
	//	delay *= ((MissileWeapon)this).speedFactor(user);
	//	if (enemy != null) {
	//		SnipersMark mark = user.buff(SnipersMark.class);
	//		if (mark != null) {
	//			if (mark.object == enemy.id()) {
	//				delay *= 0.5f;
	//			}
	//			user.remove(mark);
	//		}
	//	}
	//}
	//final float finalDelay = delay;
	//
	//((MissileSprite)user.sprite.parent.recycle(MissileSprite.class)).
	//	reset(user.pos, cell, this, new Callback(){
	//	@Override
	//	public void call() {
	//		Item.this.detach(user.belongings.backpack).onThrow(cell);
	//		user.spendAndNext(finalDelay);
	//	}
	//});
}

void Item::fix()
{
	durability = maxDurability();
}

Item* Item::identify()
{
	levelKnown = true;
	cursedKnown = true;

	return this;
}

boolean Item::collect(Bag* container)
{
	//ArrayList<Item> items = container.items;
	//
	//if (items.contains(this)) {
	//	return true;
	//}
	//
	//for (Item item : items) {
	//	if (item instanceof Bag && ((Bag)item).grab(this)) {
	//		return collect((Bag)item);
	//	}
	//}
	//
	//if (stackable) {
	//
	//	Class< ? >c = getClass();
	//	for (Item item : items) {
	//		if (item.getClass() == c) {
	//			item.quantity += quantity;
	//			item.updateQuickslot();
	//			return true;
	//		}
	//	}
	//}
	//
	//if (items.size() < container.size) {
	//
	//	if (Dungeon.hero != null && Dungeon.hero.isAlive()) {
	//		Badges.validateItemLevelAquired(this);
	//	}
	//
	//	items.add(this);
	//	QuickSlot.refresh();
	//	Collections.sort(items, itemComparator);
	//	return true;
	//
	//}
	//else {
	//
	//	GLog.n(TXT_PACK_FULL, name());
	//	return false;
	//
	//}
	return false;
}

void Item::use()
{
	//if (level > 0 && !isBroken()) {
	//	int threshold = (int)(maxDurability() * DURABILITY_WARNING_LEVEL);
	//	if (durability-- >= threshold && threshold > durability && levelKnown) {
	//		GLog.w(TXT_GONNA_BREAK, name());
	//	}
	//	if (isBroken()) {
	//		getBroken();
	//		if (levelKnown) {
	//			GLog.n(TXT_BROKEN, name());
	//			Dungeon.hero.interrupt();
	//
	//			CharSprite sprite = Dungeon.hero.sprite;
	//			PointF point = sprite.center().offset(0, -16);
	//			if (this instanceof Weapon) {
	//				sprite.parent.add(Degradation.weapon(point));
	//			}
	//			else if (this instanceof Armor) {
	//				sprite.parent.add(Degradation.armor(point));
	//			}
	//			else if (this instanceof Ring) {
	//				sprite.parent.add(Degradation.ring(point));
	//			}
	//			else if (this instanceof Wand) {
	//				sprite.parent.add(Degradation.wand(point));
	//			}
	//			Sample.INSTANCE.play(Assets.SND_DEGRADE);
	//		}
	//	}
	//}
}

void Item::updateQuickslot()
{
	//if (stackable) {
	//	Class< ? extends Item> cl = getClass();
	//	if (QuickSlot.primaryValue == cl || QuickSlot.secondaryValue == cl) {
	//		QuickSlot.refresh();
	//	}
	//}
	//else if (QuickSlot.primaryValue == this || QuickSlot.secondaryValue == this) {
	//	QuickSlot.refresh();
	//}
}

int Item::considerState(int price)
{
	if (cursed && cursedKnown) {
		price /= 2;
	}
	if (levelKnown) {
		if (level > 0) {
			price *= (level + 1);
			if (isBroken()) {
				price /= 2;
			}
		}
		else if (level < 0) {
			price /= (1 - level);
		}
	}
	if (price < 1) {
		price = 1;
	}

	return price;
}

std::string Item::status()
{
	std::stringstream ss;
	ss << quantity;
	return quantity != 1 ? ss.str() : "";
}

Item* Item::Virtual(const std::string& cl)
{
	return FactoryItem::Create(cl);
}
