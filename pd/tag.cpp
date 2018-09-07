#include "tag.h"
#include "simpleresource.h"
#include "game.h"

void Tag::createChildren()
{
	bg = Chrome::get(Chrome::Type::TAG);
	add(bg);
}

void Tag::layout()
{
	Button::layout();

	bg->x = _x;
	bg->y = _y;
	bg->size(_width, _height);
}

Tag::Tag(int color)
{
	lightness = 0;

	init();

	this->r = (color >> 16) / 255.0f;
	this->g = ((color >> 8) & 0xFF) / 255.0f;
	this->b = (color & 0xFF) / 255.0f;
}

void Tag::flash()
{
	lightness = 1.0f;
}

void Tag::update()
{
	Button::update();

	if (visible && lightness > 0.5) 
	{
		if ((lightness -= Game::elapsed) > 0.5) 
		{
			bg->ra = bg->ga = bg->ba = 2 * lightness - 1;
			bg->rm = 2 * r * (1 - lightness);
			bg->gm = 2 * g * (1 - lightness);
			bg->bm = 2 * b * (1 - lightness);
		}
		else 
		{
			bg->hardlight(r, g, b);
		}
	}
}
