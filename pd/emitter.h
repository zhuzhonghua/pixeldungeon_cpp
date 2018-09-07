#pragma once

#include "group.h"
#include "visual.h"

class Emitter :public Group{
public:
	class Factory {
	public:
		virtual void emit(Emitter* emitter, int index, float x, float y) = 0;

		virtual bool lightMode() { return false; }
	};
protected:
	bool _lightMode = false;

	Visual* _target;

	float _interval;
	int _quantity;

	int _count;
	float _time;

	virtual void emit(int index);
public:
	float x;
	float y;
	float width;
	float height;

	bool on = false;

	bool autoKill = true;

	Factory* factory;

	Emitter();
	void pos(float x, float y);
	void pos(PointF p);
	void pos(float x, float y, float width, float height);
	void pos(Visual* target);
	void burst(Factory* factory, int quantity);
	void pour(Factory* factory, float interval);
	void start(Factory* factory, float interval, int quantity);

	virtual void update();
	virtual void draw();

};