#include "exitbutton.h"
#include "icon.h"
#include "pixeldungeon.h"
#include "titlescene.h"
#include "game.h"

ExitButton::ExitButton()
{
	init();

	_width = image->width;
	_height = image->height;
}

void ExitButton::createChildren()
{
	Button::createChildren();

	image = Icons::get(Icons::EXIT);
	add(image);
}

void ExitButton::layout()
{
	Button::layout();

	image->x = _x;
	image->y = _y;
}

void ExitButton::onTouchDown()
{
	image->brightness(1.5f);
	//Sample.INSTANCE.play(Assets.SND_CLICK);
}

void ExitButton::onTouchUp()
{
	image->resetColor();
}

void ExitButton::onClick()
{
	if (dynamic_cast<TitleScene*>(Game::scene()) != NULL) 
	{
		Game::instance->finish();
	}
	else {
		PixelDungeon::switchNoFade(new TitleScene());
	}
}