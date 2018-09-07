#pragma once

#include "toucharea.h"

class DungeonTilemap;

class CellSelector :public TouchArea{
public:
	class Listener {
	public:
		virtual void onSelect(int cell) = 0;
		virtual std::string prompt() = 0;
	};

public:
	Listener* listener;

	bool enabled;

	CellSelector(DungeonTilemap* map);
	void select(int cell);
	void cancel();
private:
	float dragThreshold;
	bool dragging;
	PointF lastPos;

	bool pinching;
	TouchScreen::Touch* another;

	float startZoom;
	float startSpan;

protected:
	virtual void onClick(TouchScreen::Touch* touch);
	virtual void onTouchDown(TouchScreen::Touch* t);
	virtual void onTouchUp(TouchScreen::Touch* t);
	virtual void onDrag(TouchScreen::Touch* t);
};