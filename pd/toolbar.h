#pragma once

#include "component.h"
#include "button.h"
#include "itemsprite.h"
#include "typedefine.h"
#include "quickslot.h"
#include "cellselector.h"

class Image;

class Toolbar :public Component{
public:
	class Tool :public Button{
	private:
		static const int BGCOLOR = 0x7B8073;

	protected:
		Image* base;

		virtual void createChildren();
		virtual void layout();
		virtual void onTouchDown();
		virtual void onTouchUp();
	public:
		Tool(int x, int y, int width, int height);
		void enable(bool value);
	};

	class QuickslotTool : public Tool{
	private:
		QuickSlot* slot;
	public:
		QuickslotTool(int x, int y, int width, int height, boolean primary);
		virtual void enable(boolean value);
	protected:
		virtual void createChildren();
		virtual void layout();
	};

	class PickedUpItem : public ItemSprite{
	private:
		static const float DISTANCE;
		static const float DURATION;

		float dstX;
		float dstY;
		float left;
	public:
		PickedUpItem();
		void reset(Item* item, float dstX, float dstY);

		virtual void update();
	};
	

	virtual void update();
public:
	static CellSelector::Listener* informer;
private:
	static Toolbar* instance;

	Tool* btnWait;
	Tool* btnSearch;
	Tool* btnInfo;
	Tool* btnInventory;
	Tool* btnQuick1;
	Tool* btnQuick2;

	PickedUpItem* pickedUp;

	boolean lastEnabled;
protected:
	virtual void createChildren();
	virtual void layout();
public:
	Toolbar();

	void pickup(Item* item);

	static bool secondQuickslot();
	static void secondQuickslot(boolean value);
};