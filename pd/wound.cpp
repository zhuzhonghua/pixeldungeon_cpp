#include "wound.h"
#include "simpleresource.h"
#include "util.h"
#include "level.h"
#include "dungeontilemap.h"
#include "game.h"
#include "char.h"
#include "charsprite.h"
#include "dungeon.h"
#include "hero.h"

const float Wound::TIME_TO_FADE = 0.8f;

Wound::Wound()
:Image(Effects::get(Effects::WOUND))
{
	tag = "Wound";
	GameMath::PointFSet(&origin, width / 2, height / 2);
	//origin.set(width / 2, height / 2);
}

void Wound::reset(int p)
{
	revive();

	x = (p % Level::WIDTH) * DungeonTilemap::SIZE + (DungeonTilemap::SIZE - width) / 2;
	y = (p / Level::WIDTH) * DungeonTilemap::SIZE + (DungeonTilemap::SIZE - height) / 2;

	time = TIME_TO_FADE;
}

void Wound::update()
{
	Image::update();

	if ((time -= Game::elapsed) <= 0) 
	{
		kill();
	}
	else 
	{
		float p = time / TIME_TO_FADE;
		alpha(p);
		scale.x = 1 + p;
	}
}

void Wound::hit(Char* ch)
{
	hit(ch, 0);
}

void Wound::hit(Char* ch, float angle)
{
	Wound* w = (Wound*)ch->sprite->parent->recycle("Wound");
	if (w == NULL)
	{
		w = new Wound();
		ch->sprite->parent->add(w);
	}
	ch->sprite->parent->bringToFront(w);
	w->reset(ch->pos);
	w->angle = angle;
}

void Wound::hit(int pos)
{
	hit(pos, 0);
}

void Wound::hit(int pos, float angle)
{
	Group* parent = Dungeon::hero->sprite->parent;
	Wound* w = (Wound*)parent->recycle("Wound");
	if (w == NULL)
	{
		w = new Wound();
		parent->add(w);
	}
	parent->bringToFront(w);
	w->reset(pos);
	w->angle = angle;
}
