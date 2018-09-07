#pragma once

#include "button.h"
#include "bitmaptext.h"
#include "image.h"
#include "ninepatch.h"

class RedButton :public Button{
protected:
	BitmapText* _text;
	Image* _icon;
	NinePatch* _bg;

	virtual void createChildren();
	virtual void layout();
	virtual void onTouchDown();
	virtual void onTouchUp();
public:
	RedButton(const std::string& label);

	void enable(bool value);
	void text(const std::string& value);
	void textColor(int value);
	void icon(Image* icon);
	float reqWidth();
	float reqHeight();
};