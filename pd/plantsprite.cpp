#include "plantsprite.h"
#include "texturefilm.h"
#include "util.h"
#include "level.h"
#include "dungeontilemap.h"
#include "dungeon.h"
#include "game.h"

const float PlantSprite::DELAY = 0.2f;

TextureFilm* PlantSprite::frames;

PlantSprite::PlantSprite()
:Image(Assets::PLANTS)
{
	tag = "PlantSprite";
	state = State::NORMAL;
	pos = -1;


	if (frames == NULL) 
	{
		frames = new TextureFilm(tex, 16, 16);
	}

	GameMath::PointFSet(&origin, 8, 12);
	//origin.set(8, 12);
}

PlantSprite::PlantSprite(int image)
:PlantSprite()
{
	reset(image);
}

void PlantSprite::reset(Plant* plant)
{
	revive();

	//reset(plant.image);
	alpha(1.0f);

	//pos = plant.pos;
	x = (pos % Level::WIDTH) * DungeonTilemap::SIZE;
	y = (pos / Level::WIDTH) * DungeonTilemap::SIZE;

	state = State::GROWING;
	time = DELAY;
}

void PlantSprite::reset(int image)
{
	frame(frames->get(image));
}

void PlantSprite::update()
{
	Image::update();

	visible = pos == -1 || Dungeon::visible[pos];

	switch (state) 
	{
	case GROWING:
		if ((time -= Game::elapsed) <= 0) 
		{
			state = State::NORMAL;
			GameMath::PointFSet(&scale, 1);
			//scale.set(1);
		}
		else 
		{
			GameMath::PointFSet(&scale, 1 - time / DELAY);
			//scale.set(1 - time / DELAY);
		}
		break;
	case WITHERING:
		if ((time -= Game::elapsed) <= 0) 
		{
			Image::kill();
		}
		else 
		{
			alpha(time / DELAY);
		}
		break;
	default:
		break;
	}
}

void PlantSprite::kill()
{
	state = State::WITHERING;
	time = DELAY;
}
