#pragma once

#include "button.h"
#include "wndbag.h"
#include "typedefine.h"

class Char;
class ItemSlot;
class Image;

class QuickSlot :public Button, public WndBag::Listener{
private:
	static const String TXT_SELECT_ITEM;
	static const String QUICKSLOT1;
	static const String QUICKSLOT2;

	static QuickSlot* Primary;
	static QuickSlot* Secondary;

	Item* itemInSlot;
	ItemSlot* slot;

	Image* crossB;
	Image* crossM;

	void enableSlot();
	
public:
	static Bundlable* primaryValue;
	static Bundlable* secondaryValue;

	static Char* lastTarget;
	boolean targeting ;

	QuickSlot();

	void primary();
	void secondary();

	virtual void destroy();
	virtual void onSelect(Item* item);
	void item(Item* item);
	void enable(boolean value);
	void useTargeting();
	Item* select();

	static void refresh();
	static void target(Item* item, Char* target);
	static void cancel();
	static void save(Bundle* bundle);
	static void save(Bundle* bundle, Item* item);
	static void restore(Bundle* bundle);
	static void restore(Bundle* bundle, Item* item);
	static void compress();

	virtual boolean onLongClick();
protected:
	virtual void createChildren();
	virtual void layout();
	virtual void onClick();
	
};