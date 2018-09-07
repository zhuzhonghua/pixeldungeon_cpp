#include "stdafx.h"

#include "icontitle.h"
#include "pixelscene.h"
#include "window.h"
#include "item.h"

IconTitle::IconTitle()
:healthLvl(NAN)
{
	init();
}

IconTitle::IconTitle(Item* item)
: IconTitle(new ItemSprite(item->Image(), item->glowing()), item->toString())
{
}

IconTitle::IconTitle(Image* icon, const std::string& label)
{
	init();

	this->icon(icon);
	this->label(label);
}

void IconTitle::createChildren()
{
	imIcon = new Image();
	add(imIcon);

	tfLabel = PixelScene::createMultiline(FONT_SIZE);
	tfLabel->hardlight(Window::TITLE_COLOR);
	add(tfLabel);

	health = new HealthBar();
	add(health);
}

void IconTitle::layout()
{
	health->visible = !isnan(healthLvl);

	imIcon->x = _x;
	imIcon->y = _y;

	tfLabel->x = PixelScene::align(PixelScene::uiCamera, imIcon->x + imIcon->Width() + GAP);
	tfLabel->maxWidth = (int)(_width - tfLabel->x);
	tfLabel->measure();
	tfLabel->y = PixelScene::align(PixelScene::uiCamera,
		imIcon->height > tfLabel->Height() ?
		imIcon->y + (imIcon->Height() - tfLabel->baseLine()) / 2 :
		imIcon->y);

	if (health->visible) 
	{
		health->setRect(tfLabel->x, std::max(tfLabel->y + tfLabel->Height(), imIcon->y + imIcon->Height() - health->height()), tfLabel->maxWidth, 0);
		_height = health->bottom();
	}
	else 
	{
		_height = std::max(imIcon->y + imIcon->Height(), tfLabel->y + tfLabel->Height());
	}
}

void IconTitle::icon(Image* icon)
{
	remove(imIcon);
	add(imIcon = icon);
}

void IconTitle::label(const std::string& label)
{
	tfLabel->text(label);
}

void IconTitle::label(const std::string& label, int color)
{
	tfLabel->text(label);
	tfLabel->hardlight(color);
}

void IconTitle::color(int color)
{
	tfLabel->hardlight(color);
}

void IconTitle::Health(float value)
{
	health->Level(healthLvl = value);
	layout();
}