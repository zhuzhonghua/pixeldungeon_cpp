#pragma once

#include "image.h"

class Banner :public Image{
private:
	enum State {
		FADE_IN, STATIC, FADE_OUT
	};

	State state;

	float time;

	int color;
	float fadeTime;
	float showTime;

public:
	Banner(Image* sample);
	Banner(const std::string& tx);
	void show(int color, float fadeTime, float showTime);
	void show(int color, float fadeTime);
	virtual void update();
};