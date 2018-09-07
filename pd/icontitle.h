#pragma once

#include "component.h"
#include "image.h"
#include "bitmaptextmultiline.h"
#include "healthbar.h"
#include "item.h"

class IconTitle :public Component{
private:
	static const int FONT_SIZE = 9;

	static const int GAP = 2;

	float healthLvl;
protected:
	Image* imIcon;
	BitmapTextMultiline* tfLabel;
	HealthBar* health;
	
	virtual void createChildren();
	virtual void layout();
public:
	IconTitle();
	IconTitle(Item* item);
	IconTitle(Image* icon, const std::string& label);

	void icon(Image* icon);
	void label(const std::string& label);
	void label(const std::string& label, int color);
	void color(int color);
	void Health(float value);
};