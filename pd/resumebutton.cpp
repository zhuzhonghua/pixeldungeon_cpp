#include "resumebutton.h"
#include "icon.h"
#include "pixelscene.h"
#include "dungeon.h"
#include "hero.h"

ResumeButton::ResumeButton()
:Tag(0xCDD5C0)
{
	init();

	setSize(24, 22);

	visible = false;
}

void ResumeButton::update()
{
	bool prevVisible = visible;
	visible = (Dungeon::hero->lastAction != NULL);
	if (visible && !prevVisible) 
	{
		flash();
	}

	Tag::update();
}

void ResumeButton::createChildren()
{
	icon = Icons::get(Icons::RESUME);
	add(icon);
}

void ResumeButton::layout()
{
	Tag::layout();

	icon->x = PixelScene::align(PixelScene::uiCamera, _x + 1 + (_width - icon->width) / 2);
	icon->y = PixelScene::align(PixelScene::uiCamera, _y + (_height - icon->height) / 2);
}

void ResumeButton::onClick()
{
	Dungeon::hero->resume();
}
