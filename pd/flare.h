#pragma once

#include "visual.h"
#include "util.h"
#include <vector>

class SmartTexture;

class Flare :public Visual{
private:
	float duration;
	float lifespan;

	bool lightMode;

	SmartTexture* texture;

	FloatBuffer vertices;
	ShortBuffer indices;

	int nRays;

	void drawRays();
public:
	Flare(int nRays, float radius);
	Flare* color(int color, bool lightMode);
	Flare* show(Visual* visual, float duration);
	Flare* show(Group* parent, PointF pos, float duration);
	virtual void update();
	virtual void draw();
};