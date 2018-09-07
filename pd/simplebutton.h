#pragma once

#include "component.h"
#include "toucharea.h"

class Image;

class SimpleButton :public Component{
private:
	class TouchAreaNew :public TouchArea{
	public:
		SimpleButton* sb;
		TouchAreaNew(SimpleButton* sbp, Image* img);
	protected:
		virtual void onTouchDown(TouchScreen::Touch* touch);
		virtual void onTouchUp(TouchScreen::Touch* touch);
		virtual void onClick(TouchScreen::Touch* touch);
	};
	Image* image;

public:
	SimpleButton(Image* image);

protected:
	virtual void createChildren();
	virtual void layout();

	virtual void onClick() {};
};