#pragma once

#include "image.h"

class TextureFilm;
class BadgeBanner :public Image{
private:
	enum State
	{
		FADE_IN,STATIC,FADE_OUT
	};

	State state;

	static const float DEFAULT_SCALE;

	static const float FADE_IN_TIME;
	static const float STATIC_TIME;
	static const float FADE_OUT_TIME;

	int index;
	float time;

	static TextureFilm* atlas;

	static BadgeBanner* current;

	BadgeBanner(int index);

public:
	virtual void update();
	virtual void kill();
	static void highlight(Image* image, int index);
	static BadgeBanner* show(int image);
	static Image* image(int index);
};