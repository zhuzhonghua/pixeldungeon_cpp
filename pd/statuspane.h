#pragma once

#include "component.h"
#include "button.h"

class NinePatch;
class Image;
class Emitter;
class BitmapText;
class Compass;
class DangerIndicator;
class ResumeButton;
class BuffIndicator;

class StatusPane :public Component{
private:
	class MenuButton:public Button{
	private:
		Image* image;

	protected:
		virtual void createChildren();
		virtual void layout();
		virtual void onTouchDown();
		virtual void onTouchUp();
		virtual void onClick();
	public:
		MenuButton();
	};

	void layoutTags();
private:
	NinePatch* shield;
	Image* avatar;
	Emitter* blood;

	int lastTier;

	Image* hp;
	Image* exp;

	int lastLvl;
	int lastKeys;

	BitmapText* level;
	BitmapText* depth;
	BitmapText* keys;

	DangerIndicator* danger;
	//LootIndicator loot;
	ResumeButton* resume;
	BuffIndicator* buffs;
	Compass* compass;

	MenuButton* btnMenu;
	bool tagDanger = false;
	bool tagLoot = false;
	bool tagResume = false;
protected:
	virtual void createChildren();
	virtual void layout();
public:
	StatusPane();

	virtual void update();
};