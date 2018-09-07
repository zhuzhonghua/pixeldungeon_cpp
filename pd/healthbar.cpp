#include "healthbar.h"

HealthBar::HealthBar()
:hpBg(NULL)
, hpLvl(NULL)
{
	init();
}

void HealthBar::createChildren()
{
	hpBg = new ColorBlock(1, 1, COLOR_BG);
	add(hpBg);

	hpLvl = new ColorBlock(1, 1, COLOR_LVL);
	add(hpLvl);

	_height = HEIGHT;
}

void HealthBar::layout()
{
	hpBg->x = hpLvl->x = _x;
	hpBg->y = hpLvl->y = _y;

	hpBg->size(_width, HEIGHT);
	hpLvl->size(_width * level, HEIGHT);

	_height = HEIGHT;
}

void HealthBar::Level(float value)
{
	level = value;
	layout();
}