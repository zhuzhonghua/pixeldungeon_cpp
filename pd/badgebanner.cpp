#include "badgebanner.h"
#include "define.h"
#include "texturefilm.h"
#include "util.h"
#include "game.h"
#include "speck.h"

const float BadgeBanner::DEFAULT_SCALE = 3;

const float BadgeBanner::FADE_IN_TIME = 0.2f;
const float BadgeBanner::STATIC_TIME = 1.0f;
const float BadgeBanner::FADE_OUT_TIME = 1.0f;

TextureFilm* BadgeBanner::atlas;

BadgeBanner* BadgeBanner::current;

BadgeBanner::BadgeBanner(int index)
:Image(Assets::BADGES)
{
	if (atlas == NULL) 
	{
		atlas = new TextureFilm(tex, 16, 16);
	}

	this->index = index;

	frame(atlas->get(index));
	GameMath::PointFSet(&origin, width / 2, height / 2);
	//origin.set(width / 2, height / 2);

	alpha(0);
	GameMath::PointFSet(&scale, 2 * DEFAULT_SCALE);
	//scale.set(2 * DEFAULT_SCALE);

	state = State::FADE_IN;
	time = FADE_IN_TIME;

	//Sample.INSTANCE.play(Assets.SND_BADGE);
}

void BadgeBanner::update()
{
	Image::update();

	time -= Game::elapsed;
	if (time >= 0) 
	{
		switch (state) 
		{
		case FADE_IN:
			{
				float p = time / FADE_IN_TIME;
				GameMath::PointFSet(&scale, (1 + p) * DEFAULT_SCALE);
				//scale.set((1 + p) * DEFAULT_SCALE);
				alpha(1 - p);
			}
			break;
		case STATIC:
			break;
		case FADE_OUT:
			alpha(time / FADE_OUT_TIME);
			break;
		}

	}
	else 
	{
		switch (state) 
		{
		case FADE_IN:
			time = STATIC_TIME;
			state = State::STATIC;
			GameMath::PointFSet(&scale, DEFAULT_SCALE);
			//scale.set(DEFAULT_SCALE);
			alpha(1);
			highlight(this, index);
			break;
		case STATIC:
			time = FADE_OUT_TIME;
			state = State::FADE_OUT;
			break;
		case FADE_OUT:
			killAndErase();
			break;
		}

	}
}

void BadgeBanner::kill()
{
	if (current == this) 
	{
		current = NULL;
	}
	Image::kill();
}

void BadgeBanner::highlight(Image* image, int index)
{
	PointF p;

	switch (index) {
	case 0:
	case 1:
	case 2:
	case 3:
		GameMath::PointFOffSet(&p, 7, 3);
		break;
	case 4:
	case 5:
	case 6:
	case 7:
		GameMath::PointFOffSet(&p, 6, 5);
		break;
	case 8:
	case 9:
	case 10:
	case 11:
		GameMath::PointFOffSet(&p, 6, 3);
		break;
	case 12:
	case 13:
	case 14:
	case 15:
		GameMath::PointFOffSet(&p, 7, 4);
		break;
	case 16:
		GameMath::PointFOffSet(&p, 6, 3);
		break;
	case 17:
		GameMath::PointFOffSet(&p, 5, 4);
		break;
	case 18:
		GameMath::PointFOffSet(&p, 7, 3);
		break;
	case 20:
		GameMath::PointFOffSet(&p, 7, 3);
		break;
	case 21:
		GameMath::PointFOffSet(&p, 7, 3);
		break;
	case 22:
		GameMath::PointFOffSet(&p, 6, 4);
		break;
	case 23:
		GameMath::PointFOffSet(&p, 4, 5);
		break;
	case 24:
		GameMath::PointFOffSet(&p, 6, 4);
		break;
	case 25:
		GameMath::PointFOffSet(&p, 6, 5);
		break;
	case 26:
		GameMath::PointFOffSet(&p, 5, 5);
		break;
	case 27:
		GameMath::PointFOffSet(&p, 6, 4);
		break;
	case 28:
		GameMath::PointFOffSet(&p, 3, 5);
		break;
	case 29:
		GameMath::PointFOffSet(&p, 5, 4);
		break;
	case 30:
		GameMath::PointFOffSet(&p, 5, 4);
		break;
	case 31:
		GameMath::PointFOffSet(&p, 5, 5);
		break;
	case 32:
	case 33:
		GameMath::PointFOffSet(&p, 7, 4);
		break;
	case 34:
		GameMath::PointFOffSet(&p, 6, 4);
		break;
	case 35:
		GameMath::PointFOffSet(&p, 6, 4);
		break;
	case 36:
		GameMath::PointFOffSet(&p, 6, 5);
		break;
	case 37:
		GameMath::PointFOffSet(&p, 4, 4);
		break;
	case 38:
		GameMath::PointFOffSet(&p, 5, 5);
		break;
	case 39:
		GameMath::PointFOffSet(&p, 5, 4);
		break;
	case 40:
	case 41:
	case 42:
	case 43:
		GameMath::PointFOffSet(&p, 5, 4);
		break;
	case 44:
	case 45:
	case 46:
	case 47:
		GameMath::PointFOffSet(&p, 5, 5);
		break;
	case 48:
	case 49:
	case 50:
	case 51:
		GameMath::PointFOffSet(&p, 7, 4);
		break;
	case 52:
	case 53:
	case 54:
	case 55:
		GameMath::PointFOffSet(&p, 4, 4);
		break;
	case 56:
		GameMath::PointFOffSet(&p, 3, 7);
		break;
	case 57:
		GameMath::PointFOffSet(&p, 4, 5);
		break;
	case 58:
		GameMath::PointFOffSet(&p, 6, 4);
		break;
	case 59:
		GameMath::PointFOffSet(&p, 7, 4);
		break;
	case 60:
	case 61:
	case 62:
	case 63:
		GameMath::PointFOffSet(&p, 4, 4);
		break;
	}

	p.x *= image->scale.x;
	p.y *= image->scale.y;
	GameMath::PointFOffSet(&p, 
		-image->origin.x * (image->scale.x - 1),
		-image->origin.y * (image->scale.y - 1));
	GameMath::PointFOffSet(&p, image->point());

	Speck* star = new Speck();
	star->reset(0, p.x, p.y, Speck::DISCOVER);
	star->cameraf = image->camera();
	image->parent->add(star);
}

BadgeBanner* BadgeBanner::show(int image)
{
	if (current != NULL) 
	{
		current->killAndErase();
	}
	return (current = new BadgeBanner(image));
}

Image* BadgeBanner::image(int index)
{
	Image* image = new Image(Assets::BADGES);
	if (atlas == NULL) 
	{
		atlas = new TextureFilm(image->tex, 16, 16);
	}
	image->frame(atlas->get(index));
	return image;
}
