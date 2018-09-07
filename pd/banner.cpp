#include "banner.h"
#include "game.h"

Banner::Banner(Image* sample)
{
	copy(sample);
	alpha(0);
	delete sample;
}

Banner::Banner(const std::string& tx)
:Image(tx)
{
	alpha(0);
}

void Banner::show(int color, float fadeTime, float showTime)
{
	this->color = color;
	this->fadeTime = fadeTime;
	this->showTime = showTime;

	state = State::FADE_IN;

	time = fadeTime;
}

void Banner::show(int color, float fadeTime)
{
	show(color, fadeTime, std::numeric_limits<float>::max());
}

void Banner::update()
{
	Image::update();

	time -= Game::elapsed;
	if (time >= 0) {

		float p = time / fadeTime;

		switch (state) {
		case FADE_IN:
			tint(color, p);
			alpha(1 - p);
			break;
		case STATIC:
			break;
		case FADE_OUT:
			alpha(p);
			break;
		}

	}
	else {

		switch (state) {
		case FADE_IN:
			time = showTime;
			state = State::STATIC;
			break;
		case STATIC:
			time = fadeTime;
			state = State::FADE_OUT;
			break;
		case FADE_OUT:
			killAndErase();
			delete this;
			break;
		}

	}
}
