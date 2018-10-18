#pragma once

#include "component.h"
#include "toucharea.h"

class Button;
namespace{
	
};

class Button : public Component{
	class TouchArea1 :public TouchArea{
	public:
		TouchArea1(Button* btn);
	protected:
		Button* _btn;
		virtual void onTouchDown(TouchScreen::Touch* touch);
		virtual void onTouchUp(TouchScreen::Touch* touch);
		virtual void onClick(TouchScreen::Touch* touch);
	};
public:
	static float longClick;

	Button();

	virtual void update();
protected:
	TouchArea* hotArea;

	bool _pressed;
	float _pressTime;

	bool _processed;

	virtual void onTouchDown() {};
	virtual void onTouchUp() {};
	virtual void onClick() {};

	virtual bool onLongClick() { return false; };

	virtual void layout();
	virtual void createChildren();
};