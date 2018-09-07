#pragma once

#include "component.h"

class Char;
class Image;

class HealthIndicator :public Component{
private:
	static const float HEIGHT;

	Char* target;

	Image* bg;
	Image* level;
public:
	static HealthIndicator* instance;

	HealthIndicator();

	virtual void update();
	void Target(Char* ch);
	Char* Target() { return target; }
protected:
	virtual void createChildren();
};

class BitmapText;

class GoldIndicator :public Component{
private:
	static const float TIME;

	int lastValue;

	BitmapText* tf;

	float time;

public:
	GoldIndicator();
	virtual void update();
protected:
	virtual void createChildren();
	virtual void layout();
};