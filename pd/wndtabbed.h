#pragma once

#include "window.h"
#include "button.h"
#include "ninepatch.h"
#include "bitmaptext.h"

class WndTabbed :public Window{
protected:
	class Tab :public Button{
		friend class WndTabbed;
	public:
		WndTabbed* wnd;
		Tab(WndTabbed* wnd);
	protected:
		const int CUT = 5;

		bool selected;

		NinePatch* bg;

		virtual void layout();

		virtual void select(bool value);

		virtual void onClick();
	};
public:
	class LabeledTab :public Tab{
	private:
		BitmapText* btLabel;
	public:
		LabeledTab(WndTabbed* wnd, const std::string& label);
	protected:
		virtual void createChildren();
		virtual void layout();
		virtual void select(bool value);
	};

protected:
	void onClick(Tab* tab);
	int tabHeight() { return 25; }
	Tab* add(Tab* tab);

	std::vector<Tab*> tabs;
	Tab* selected;
public:
	virtual void resize(int w, int h);

	void select(Tab* tab);
	void select(int index);

	WndTabbed();
};

class Buff;
class TextureFilm;

class WndHero :public WndTabbed{
private:
	class StatsTab :public Group{
	private:
		static const String TXT_TITLE;
		static const String TXT_CATALOGUS;
		static const String TXT_JOURNAL;

		static const int GAP = 5;

		float pos;

		void statSlot(const String& label, const String& value);
		void statSlot(const String& label, int value);
	public:
		WndHero* wh;
		StatsTab(WndHero* w);
		float height() {
			return pos;
		}
	};

	class BuffsTab :public Group{
	private:
		static const int GAP = 2;

		float pos;

		void buffSlot(Buff* buff);
	public:
		WndHero* wh;
		BuffsTab(WndHero* w);
		float height() {
			return pos;
		}
	};
private:
	static const String TXT_STATS;
	static const String TXT_BUFFS;

	static const String TXT_EXP;
	static const String TXT_STR;
	static const String TXT_HEALTH;
	static const String TXT_GOLD;
	static const String TXT_DEPTH;

	static const int WIDTH = 100;
	static const int TAB_WIDTH = 40;

	StatsTab* stats;
	BuffsTab* buffs;

	SmartTexture* icons;
	TextureFilm* film;

	
};

class ItemSprite;
class ScrollPane;

class WndCatalogus :public WndTabbed{
public:
	class ListItem :public Component{
	private:
		Item* item;
		boolean identified;

		ItemSprite* sprite;
		BitmapText* label;

	public:
		ListItem(const std::string& cl);
		boolean onClick(float x, float y);
	protected:
		virtual void createChildren();
		virtual void layout();
	};
private:
	static const int WIDTH_P = 112;
	static const int HEIGHT_P = 160;

	static const int WIDTH_L = 128;
	static const int HEIGHT_L = 128;

	static const int ITEM_HEIGHT = 18;

	static const int TAB_WIDTH = 50;

	static const String TXT_POTIONS;
	static const String TXT_SCROLLS;
	static const String TXT_TITLE;

	BitmapText* txtTitle;
	ScrollPane* list;

public:
	static boolean showPotions;

	void updateList();

	ArrayList<ListItem*> items;
	WndCatalogus();
};