#pragma once

#include "visual.h"
#include "touchscreen.h"

class TouchArea :public Visual,public Signal<TouchScreen::Touch>::Listener{
public:
	Visual* target;
	TouchArea(Visual* target);
	TouchArea(float x, float y, float width, float height);
	virtual ~TouchArea();
	void reset() { _touch = NULL; }

	virtual void onSignal(TouchScreen::Touch* touch);

	virtual void destroy();
protected:
	virtual void onTouchDown(TouchScreen::Touch* touch) {}
	virtual void onTouchUp(TouchScreen::Touch* touch) {}
	virtual void onClick(TouchScreen::Touch* touch) {}
	virtual void onDrag(TouchScreen::Touch* touch) {}

	void _init();
	TouchScreen::Touch* _touch;
};