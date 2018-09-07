#pragma once

#include "group.h"

class Image;
class Callback;

class Lightning :public Group{
private:
	static const float DURATION;

	float life;

	int length;
	std::vector<float> cx;
	std::vector<float> cy;

	std::vector<Image*> arcsS;
	std::vector<Image*> arcsE;

	Callback* callback;

	static const double A;

public:
	Lightning(std::vector<int> cells, int length, Callback* callback);

	virtual void update();
	virtual void draw();
};