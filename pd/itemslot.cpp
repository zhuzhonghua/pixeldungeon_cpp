#include "itemslot.h"
#include "item.h"
#include "simpleresource.h"
#include "pixelscene.h"
#include "armor.h"
#include "weapon.h"
#include "dungeon.h"
#include "hero.h"

namespace{
	class ItemChest :public Item{
	public:
		virtual int image() { return ItemSpriteSheet::CHEST; }
	};
	class ItemLockedChest :public Item{
	public:
		virtual int image() { return ItemSpriteSheet::LOCKED_CHEST; }
	};
	class ItemTomb :public Item{
	public:
		virtual int image() { return ItemSpriteSheet::TOMB; }
	};
	class ItemSkeleton :public Item{
	public:
		virtual int image() { return ItemSpriteSheet::BONES; }
	};
}
Item* ItemSlot::CHEST = new ItemChest();
Item* ItemSlot::LOCKED_CHEST = new ItemLockedChest();
Item* ItemSlot::TOMB = new ItemTomb();
Item* ItemSlot::SKELETON = new ItemSkeleton();

ItemSlot::ItemSlot()
{
	init();
}

ItemSlot::ItemSlot(Item* item)
:ItemSlot()
{
	this->item(item);
}

void ItemSlot::item(Item* item)
{
	if (item == NULL) {

		active = false;
		icon->visible = topLeft->visible = topRight->visible = bottomRight->visible = false;

	}
	else {

		active = true;
		icon->visible = topLeft->visible = topRight->visible = bottomRight->visible = true;

		icon->view(item->Image(), item->glowing());

		topLeft->text(item->status());

		boolean isArmor = dynamic_cast<Armor*>(item) != NULL;
		boolean isWeapon = dynamic_cast<Weapon*>(item) != NULL;
		if (isArmor || isWeapon) {

			if (item->levelKnown || (isWeapon && !(dynamic_cast<MeleeWeapon*>(item) != NULL))) {

				int str = isArmor ? ((Armor*)item)->STR : ((Weapon*)item)->STR;
				topRight->text(GameMath::format(TXT_STRENGTH.c_str(), str));
				if (str > Dungeon::hero->sTR()) {
					topRight->hardlight(DEGRADED);
				}
				else {
					topRight->resetColor();
				}

			}
			else {

				topRight->text(GameMath::format(TXT_TYPICAL_STR.c_str(), isArmor ?
					((Armor*)item)->typicalSTR() :
					((MeleeWeapon*)item)->typicalSTR()));
				topRight->hardlight(WARNING);

			}
			topRight->measure();

		}
		else {

			topRight->text("");

		}

		int level = item->visiblyUpgraded();
		if (level != 0 || (item->cursed && item->cursedKnown)) {
			bottomRight->text(item->levelKnown ? GameMath::format(TXT_LEVEL.c_str(), level) : TXT_CURSED);
			bottomRight->measure();
			bottomRight->hardlight(level > 0 ? (item->isBroken() ? WARNING : UPGRADED) : DEGRADED);
		}
		else {
			bottomRight->text("");
		}

		layout();
	}
}

void ItemSlot::enable(boolean value)
{
	active = value;

	float alpha = value ? ENABLED : DISABLED;
	icon->alpha(alpha);
	topLeft->alpha(alpha);
	topRight->alpha(alpha);
	bottomRight->alpha(alpha);
}

void ItemSlot::showParams(boolean value)
{
	if (value) {
		add(topRight);
		add(bottomRight);
	}
	else {
		remove(topRight);
		remove(bottomRight);
	}
}

const float ItemSlot::ENABLED = 1.0f;
const float ItemSlot::DISABLED = 0.3f;
const String ItemSlot::TXT_STRENGTH = ":%d";
const String ItemSlot::TXT_TYPICAL_STR = "%d?";
const String ItemSlot::TXT_LEVEL = "%+d";
const String ItemSlot::TXT_CURSED = "";//"-";

void ItemSlot::createChildren()
{
	icon = new ItemSprite();
	add(icon);

	topLeft = PixelScene::createText(9);
	add(topLeft);

	topRight = PixelScene::createText(9);
	add(topRight);

	bottomRight = PixelScene::createText(9);
	add(bottomRight);
}

void ItemSlot::layout()
{
	Button::layout();

	icon->x = _x + (_width - icon->width) / 2;
	icon->y = _y + (_height - icon->height) / 2;

	if (topLeft != NULL) {
		topLeft->x = _x;
		topLeft->y = _y;
	}

	if (topRight != NULL) {
		topRight->x = _x + (_width - topRight->Width());
		topRight->y = _y;
	}

	if (bottomRight != NULL) {
		bottomRight->x = _x + (_width - bottomRight->Width());
		bottomRight->y = _y + (_height - bottomRight->Height());
	}
}
