#include "healthindicator.h"
#include "image.h"
#include "texturecache.h"
#include "char.h"
#include "charsprite.h"
#include "pixelscene.h"
#include "game.h"
#include "dungeon.h"

const float HealthIndicator::HEIGHT = 2;

HealthIndicator* HealthIndicator::instance;

HealthIndicator::HealthIndicator()
{
	target = NULL;

	init();

	instance = this;
}

void HealthIndicator::update()
{
	Component::update();

	if (target != NULL && target->isAlive() && target->sprite->visible) 
	{
		CharSprite* sprite = target->sprite;
		bg->scale.x = sprite->width;
		level->scale.x = sprite->width * target->HP / target->HT;
		bg->x = level->x = sprite->x;
		bg->y = level->y = sprite->y - HEIGHT - 1;

		visible = true;
	}
	else 
	{
		visible = false;
	}
}

void HealthIndicator::Target(Char* ch)
{
	if (ch != NULL && ch->isAlive()) 
	{
		target = ch;
	}
	else 
	{
		target = NULL;
	}
}

void HealthIndicator::createChildren()
{
	bg = new Image(TextureCache::createSolid(0xFFcc0000));
	bg->scale.y = HEIGHT;
	add(bg);

	level = new Image(TextureCache::createSolid(0xFF00cc00));
	level->scale.y = HEIGHT;
	add(level);
}

const float GoldIndicator::TIME = 2.0f;

GoldIndicator::GoldIndicator()
{
	lastValue = 0;
	init();
}

void GoldIndicator::update()
{
	Component::update();

	if (visible) {

		time -= Game::elapsed;
		if (time > 0) {
			tf->alpha(time > TIME / 2 ? 1.0f : time * 2 / TIME);
		}
		else {
			visible = false;
		}

	}

	if (Dungeon::gold != lastValue) {

		lastValue = Dungeon::gold;

		std::stringstream ss;
		ss << lastValue;

		tf->text(ss.str());
		tf->measure();

		visible = true;
		time = TIME;

		layout();
	}
}

void GoldIndicator::createChildren()
{
	tf = PixelScene::createText(9);
	tf->hardlight(0xFFFF00);
	add(tf);

	visible = false;
}

void GoldIndicator::layout()
{
	tf->x = _x + (_width - tf->Width()) / 2;
	tf->y = bottom() - tf->Height();
}
