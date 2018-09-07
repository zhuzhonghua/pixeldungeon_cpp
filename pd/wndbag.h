#pragma once

#include "wndtabbed.h"

class Item;
class Bag;

class WndBag :public WndTabbed{
public:
	enum Mode {
		ALL,
		UNIDENTIFED,
		UPGRADEABLE,
		QUICKSLOT,
		FOR_SALE,
		WEAPON,
		ARMOR,
		ENCHANTABLE,
		WAND,
		SEED
	};

	class Listener {
	public:
		virtual void onSelect(Item* item) = 0;
	};

protected:
	static const int COLS_P = 4;
	static const int COLS_L = 6;

	static const int SLOT_SIZE = 28;
	static const int SLOT_MARGIN = 1;

	static const int TAB_WIDTH = 25;

	static const int TITLE_HEIGHT = 12;

private:
	Listener* listener;
	WndBag::Mode mode;
	std::string title;

	int nCols;
	int nRows;

protected:
	int count;
	int col;
	int row;

private:
	static Mode lastMode;
	static Bag* lastBag;

public:
	WndBag(Bag* bag, Listener* listener, Mode mode, const std::string& title);
	static WndBag* seedPouch(Listener* listener, Mode mode, const String& title);
	static WndBag* LastBag(Listener* listener, Mode mode, const String& title);
protected:
	void placeItems(Bag* container);
};