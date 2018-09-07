#include "quickslot.h"
#include "itemslot.h"
#include "gamescene.h"
#include "image.h"
#include "dungeon.h"
#include "icon.h"
#include "belongings.h"
#include "bag.h"
#include "mob.h"
#include "charsprite.h"
#include "dungeontilemap.h"
#include "healthindicator.h"
#include "toolbar.h"
#include "hero.h"

const String QuickSlot::TXT_SELECT_ITEM = "Select an item for the quickslot";

QuickSlot* QuickSlot::Primary;

QuickSlot* QuickSlot::Secondary;

Char* QuickSlot::lastTarget;

void QuickSlot::primary()
{
	Primary = this;
	item(select());
}

void QuickSlot::secondary()
{
	Secondary = this;
	item(select());
}

void QuickSlot::destroy()
{
	Button::destroy();

	if (this == Primary) {
		Primary = NULL;
	}
	else {
		Secondary = NULL;
	}

	lastTarget = NULL;
}

QuickSlot::QuickSlot()
{
	targeting = false;

	init();
}

namespace{
	class ItemSlotNew :public ItemSlot{
	public:
		QuickSlot* qs;
		ItemSlotNew(QuickSlot* q) :qs(q){}

	protected:
		virtual void onClick()
		{
			if (qs->targeting) {
				GameScene::handleCell(QuickSlot::lastTarget->pos);
			}
			else {
				qs->useTargeting();
				qs->select()->execute(Dungeon::hero, "");
			}
		}
		virtual boolean onLongClick() {
			return qs->onLongClick();
		}
		virtual void onTouchDown() {
			icon->lightness(0.7f);
		}
		virtual void onTouchUp() {
			icon->resetColor();
		}
	};
}
void QuickSlot::createChildren()
{
	slot = new ItemSlotNew(this);
	add(slot);

	crossB = Icons::get(Icons::TARGET);
	crossB->visible = false;
	add(crossB);

	crossM = new Image();
	crossM->copy(crossB);
}

void QuickSlot::layout()
{
	Button::layout();

	slot->fill(this);

	crossB->x = PixelScene::align(_x + (_width - crossB->width) / 2);
	crossB->y = PixelScene::align(_y + (_height - crossB->height) / 2);
}

void QuickSlot::onClick()
{
	GameScene::selectItem(this, WndBag::Mode::QUICKSLOT, TXT_SELECT_ITEM);
}

boolean QuickSlot::onLongClick()
{
	GameScene::selectItem(this, WndBag::Mode::QUICKSLOT, TXT_SELECT_ITEM);
	return true;
}

Item* QuickSlot::select()
{
	Bundlable* content = (this == Primary ? primaryValue : secondaryValue);
	if (dynamic_cast<Item*>(content)) {
	
		return (Item*)content;
	
	}
	else if (content != NULL) {
	
		// TODO: Check Every Item SubClass's getObject
		Item* item = Dungeon::hero->belongings->getItem(content->getClassName());
		return item != NULL ? item : Item::Virtual(content->getClassName());
	
	}
	else {

		return NULL;

	}
}

void QuickSlot::onSelect(Item* item)
{
	if (item != NULL) {
		if (this == Primary) {
			primaryValue = (item->stackable ? FactoryItem::Create(item->getClassName()) : item);
		}
		else {
			secondaryValue = (item->stackable ? FactoryItem::Create(item->getClassName()) : item);
		}
		refresh();
	}
}

void QuickSlot::item(Item* item)
{
	slot->item(item);
	itemInSlot = item;
	enableSlot();
}

void QuickSlot::enable(boolean value)
{
	active = value;
	if (value) {
		enableSlot();
	}
	else {
		slot->enable(false);
	}
}

void QuickSlot::enableSlot()
{
	slot->enable(
		itemInSlot != NULL &&
		itemInSlot->Quantity() > 0 &&
		(Dungeon::hero->belongings->backpack->contains(itemInSlot) || itemInSlot->isEquipped(Dungeon::hero)));
}

void QuickSlot::useTargeting()
{
	targeting = lastTarget != NULL && lastTarget->isAlive() && Dungeon::visible[lastTarget->pos];

	if (targeting) {
		int pos = Ballistica::cast(Dungeon::hero->pos, lastTarget->pos, false, true);
		if (pos != lastTarget->pos) {
			lastTarget = NULL;
			targeting = false;
		}
	}

	if (!targeting) {
		int n = Dungeon::hero->visibleEnemies.size();
		for (int i = 0; i < n; i++) {
			Mob* enemy = Dungeon::hero->visibleEnemy(i);
			int pos = Ballistica::cast(Dungeon::hero->pos, enemy->pos, false, true);
			if (pos == enemy->pos) {
				lastTarget = enemy;
				targeting = true;
				break;
			}
		}
	}

	if (targeting) {
		if (Actor::all.contains(lastTarget)) {
			lastTarget->sprite->parent->add(crossM);
			crossM->point(DungeonTilemap::tileToWorld(lastTarget->pos));
			crossB->visible = true;
		}
		else {
			lastTarget = NULL;
		}
	}
}

void QuickSlot::refresh()
{
	if (Primary != NULL) {
		Primary->item(Primary->select());
	}
	if (Secondary != NULL) {
		Secondary->item(Secondary->select());
	}
}

void QuickSlot::target(Item* item, Char* target)
{
	if (target != Dungeon::hero) {
		lastTarget = target;
		HealthIndicator::instance->Target(target);
	}
}

void QuickSlot::cancel()
{
	if (Primary != NULL && Primary->targeting) {
		Primary->crossB->visible = false;
		Primary->crossM->remove();
		Primary->targeting = false;
	}
	if (Secondary != NULL && Secondary->targeting) {
		Secondary->crossB->visible = false;
		Secondary->crossM->remove();
		Secondary->targeting = false;
	}
}

void QuickSlot::save(Bundle* bundle)
{
	Belongings* stuff = Dungeon::hero->belongings;
	
	if (primaryValue  &&
		stuff->getItem(primaryValue->getClassName()) != NULL) {
	
		bundle->put(QUICKSLOT1, primaryValue->getClassName());
	}
	if (QuickSlot::secondaryValue &&
		stuff->getItem(secondaryValue->getClassName()) != NULL &&
		Toolbar::secondQuickslot()) {
	
		bundle->put(QUICKSLOT2, secondaryValue->getClassName());
	}
}

void QuickSlot::save(Bundle* bundle, Item* item)
{
	if (item == primaryValue) {
		bundle->put(QuickSlot::QUICKSLOT1, true);
	}
	if (item == secondaryValue && Toolbar::secondQuickslot()) {
		bundle->put(QuickSlot::QUICKSLOT2, true);
	}
}

void QuickSlot::restore(Bundle* bundle)
{
	primaryValue = NULL;
	secondaryValue = NULL;
	
	String qsClass = bundle->getString(QUICKSLOT1);
	if (qsClass.size() > 0) {
		primaryValue = FactoryItem::Create(qsClass);
	}
	
	qsClass = bundle->getString(QUICKSLOT2);
	if (qsClass.size() > 0) {
		secondaryValue = FactoryItem::Create(qsClass);
	}
}

void QuickSlot::restore(Bundle* bundle, Item* item)
{
	if (bundle->getBoolean(QUICKSLOT1)) {
		primaryValue = item;
	}
	if (bundle->getBoolean(QUICKSLOT2)) {
		secondaryValue = item;
	}
}

void QuickSlot::compress()
{
	if ((primaryValue == NULL && secondaryValue != NULL) ||
		(primaryValue == secondaryValue)) {
	
		primaryValue = secondaryValue;
		secondaryValue = NULL;
	}
}

Bundlable* QuickSlot::secondaryValue;

Bundlable* QuickSlot::primaryValue;

const String QuickSlot::QUICKSLOT2 = "quickslot2";

const String QuickSlot::QUICKSLOT1 = "quickslot";
