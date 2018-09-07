#include "ripple.h"
#include "level.h"
#include "dungeontilemap.h"
#include "util.h"
#include "game.h"
#include "simpleresource.h"

const float Ripple::TIME_TO_FADE = 0.5f;

Ripple::Ripple()
:time(0)
, Image(Effects::get(Effects::RIPPLE))
{
	tag = "Ripple";
}

void Ripple::reset(int p)
{
	revive();

	x = (p % Level::WIDTH) * DungeonTilemap::SIZE;
	y = (p / Level::WIDTH) * DungeonTilemap::SIZE;

	GameMath::PointFSet(&origin, width / 2, height / 2);
	//origin.set(width / 2, height / 2);
	GameMath::PointFSet(&scale, 0);
	//scale.set(0);

	time = TIME_TO_FADE;
}

void Ripple::update()
{
	Image::update();

	if ((time -= Game::elapsed) <= 0) 
	{
		kill();
	}
	else 
	{
		float p = time / TIME_TO_FADE;
		GameMath::PointFSet(&scale, 1 - p);
		//scale.set(1 - p);
		alpha(p);
	}
}