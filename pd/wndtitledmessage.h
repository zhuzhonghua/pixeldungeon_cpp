#pragma once

#include "window.h"
#include "image.h"
#include "component.h"
#include "charsprite.h"
#include "mob.h"

class WndTitledMessage :public Window{
private:
	static const int WIDTH_P = 120;
	static const int WIDTH_L = 144;

	static const int GAP = 2;

public:
	WndTitledMessage(Image* icon, const std::string& title, const std::string& message);
	WndTitledMessage(Component* titlebar, const std::string& message);
};

class HealthBar;
class BuffIndicator;

class WndInfoMob :public WndTitledMessage{
private:
	class MobTitle :public Component{
	private:
		static const int GAP = 2;

		CharSprite* image;
		BitmapText* name;
		HealthBar* health;
		BuffIndicator* buffs;

	public:
		MobTitle(Mob* mob);

	protected:
		virtual void layout();
	};

	static String desc(Mob* mob);
public:
	WndInfoMob(Mob* mob);
};