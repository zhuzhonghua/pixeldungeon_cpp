#pragma once

#include "button.h"
#include "typedefine.h"

class ItemSprite;
class BitmapText;
class Item;

class ItemSlot :public Button{
public:
	static const int DEGRADED = 0xFF4444;
	static const int UPGRADED = 0x44FF44;
	static const int WARNING = 0xFF8800;

	static Item* CHEST;
	static Item* LOCKED_CHEST;
	static Item* TOMB;
	static Item* SKELETON;

	ItemSlot();
	ItemSlot(Item* item);

	void item(Item* item);
	void enable(boolean value);
	void showParams(boolean value);
private:
	static const float ENABLED;
	static const float DISABLED;

	static const String TXT_STRENGTH;
	static const String TXT_TYPICAL_STR;

	static const String TXT_LEVEL;
	static const String TXT_CURSED;

protected:
	ItemSprite* icon;
	BitmapText* topLeft;
	BitmapText* topRight;
	BitmapText* bottomRight;

	virtual void createChildren();
	virtual void layout();
};