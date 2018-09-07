#pragma once

#include "component.h"
#include "toucharea.h"
#include "typedefine.h"

class ColorBlock;

class ScrollPane :public Component{
public:
	class TouchController :public TouchArea{
	private:
		float dragThreshold;
		boolean dragging;
		PointF lastPos;
	protected:
		virtual void onClick(TouchScreen::Touch* touch);
		virtual void onDrag(TouchScreen::Touch* t);
	public:
		ScrollPane* sp;
		TouchController(ScrollPane* s);
	};
protected:
	static const int THUMB_COLOR = 0xFF7b8073;
	static const float THUMB_ALPHA;

	TouchController* controller;
	Component* content;
	ColorBlock* thumb;

	float minX;
	float minY;
	float maxX;
	float maxY;

	virtual void createChildren();
	virtual void layout();
public:
	ScrollPane(Component* content);

	virtual void destroy();
	void scrollTo(float x, float y);
	Component* Content() {
		return content;
	}

	virtual void onClick(float x, float y) {
	}

};