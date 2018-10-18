#pragma once

#include "define.h"

#include "gizmo.h"

class Visual :public Gizmo{
public:
	float x;
	float y;
	float width;
	float height;

	PointF scale;
	PointF origin;

	float rm;
	float gm;
	float bm;
	float am;
	float ra;
	float ga;
	float ba;
	float aa;

	PointF speed;
	PointF acc;

	float angle;
	float angularSpeed;

	Visual(float x, float y, float width, float height);
	virtual void update();
	virtual void draw();
	PointF point() { return PointF(x,y); }
	PointF point(PointF p) { x = p.x, y = p.y; return p; }
	Point point(Point p) { x = p.x, y = p.y; return p; }
	PointF center() { return PointF(x + width / 2, y + height / 2); }
	PointF center(PointF p) { x = p.x - width / 2; y = p.y - height / 2; return p; }
	float getWidth() { return width*scale.x; }
	float getHeight() { return height*scale.y; }
	void alpha(float v) { am = v; aa = 0; }
	float alpha() { return am + aa; }
	void invert() { rm = gm = bm = -1.0f; ra = ga = ba = +1.0f; }

	void brightness(float value) { rm = gm = bm = value; }
	void lightness(float value);
	void tint(float r, float g, float b, float strength);
	void tint(int color, float strength);
	void color(float r, float g, float b);
	void color(int color);
	void hardlight(float r, float g, float b);
	void hardlight(int color);
	void resetColor();
	bool overlapsPoint(float x, float y);
	bool overlapsScreenPoint(int x, int y);
	virtual bool isVisible();
protected:
	Matrix	_mat;

	void updateMotion();
	virtual void updateMatrix();
};