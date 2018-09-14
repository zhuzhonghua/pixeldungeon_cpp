#pragma once

#include "component.h"
#include "toucharea.h"

class Button;
namespace{
	class TouchArea1 :public TouchArea{
	public:
		TouchArea1(Button* btn);
	protected:
		Button* _btn;
		virtual void onTouchDown(TouchScreen::Touch* touch);
		virtual void onTouchUp(TouchScreen::Touch* touch);
		virtual void onClick(TouchScreen::Touch* touch);
	};
};

class Button : public Component{
public:
	static float longClick;

	Button();

	virtual void update();
protected:
	TouchArea* hotArea;

	friend class TouchArea1;

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