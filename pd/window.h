#pragma once

#include "group.h"
#include "signal.h"
#include "keys.h"
#include "ninepatch.h"
#include "toucharea.h"
#include "shadowbox.h"
#include "typedefine.h"
#include "component.h"
#include "itemsprite.h"
#include "journal.h"

class Window:public Group, public Signal<Keys::Key>::Listener{
public:
	int width;
	int height;

	TouchArea* blocker;
	ShadowBox* shadow;
	NinePatch* chrome;

public:
	static const int TITLE_COLOR = 0xFFFF44;

	Window();
	Window(int width, int height);
	Window(int width, int height, NinePatch* chrome);

	virtual void onBackPressed() 
	{
		hide();

		delete this;
	}
	void hide() 
	{
		parent->erase(this);
		//destroy();
		// ATTENTION: call hide, and then delete
	}

	virtual void resize(int w, int h);
	virtual void destroy();

	virtual void onSignal(Keys::Key* key);
};

class WndInfoCell :public Window{
private:
	static const float GAP;

	static const int WIDTH = 120;

	static const String TXT_NOTHING;

public:
	WndInfoCell(int cell);
};

class BitmapText;
class Image;
class ScrollPane;

class WndJournal :public Window{
private:
	class ListItem :public Component{
	private:
		BitmapText* feature;
		BitmapText* depth;

		Image* icon;

	public:
		ListItem(Journal::Feature f, int d);

	protected:
		virtual void createChildren();
		virtual void layout();
	};
private:
	static const int WIDTH = 112;
	static const int HEIGHT_P = 160;
	static const int HEIGHT_L = 144;

	static const int ITEM_HEIGHT = 18;

	static const String TXT_TITLE;

	BitmapText* txtTitle;
	ScrollPane* list;

public:
	WndJournal();
};

class Heap;

class WndInfoItem :public Window{
private:
	static const String TXT_CHEST;
	static const String TXT_LOCKED_CHEST;
	static const String TXT_CRYSTAL_CHEST;
	static const String TXT_TOMB;
	static const String TXT_SKELETON;
	static const String TXT_WONT_KNOW;
	static const String TXT_NEED_KEY;
	static const String TXT_INSIDE;
	static const String TXT_OWNER;
	static const String TXT_REMAINS;

	static const float GAP;

	static const int WIDTH = 120;

	void fillFields(int image, ItemSprite::Glowing* glowing, int titleColor, const String& title, const String& info);
public:
	WndInfoItem(Heap* heap);
	WndInfoItem(Item* item);
};

class WndBag;

class WndTradeItem :public Window{
private:
	static const float GAP;
	static const int WIDTH = 120;
	static const int BTN_HEIGHT = 16;

	static const String TXT_SALE;
	static const String TXT_BUY;
	static const String TXT_SELL;
	static const String TXT_SELL_1;
	static const String TXT_SELL_ALL;
	static const String TXT_CANCEL;

	static const String TXT_SOLD;
	static const String TXT_BOUGHT;

	WndBag* owner;

	float createDescription(Item* item, boolean forSale);
	
	int price(Item* item);
public:
	WndTradeItem(Item* item, WndBag* owner);
	WndTradeItem(Heap* heap, bool canBuy);

	virtual void hide();
	void sell(Item* item);
	void sellOne(Item* item);
	void buy(Heap* heap);
};