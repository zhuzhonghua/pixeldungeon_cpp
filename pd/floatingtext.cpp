#include "floatingtext.h"
#include "dungeontilemap.h"
#include "game.h"
#include "pixelscene.h"
#include "gamescene.h"

const float FloatingText::LIFESPAN = 1.0f;
const float FloatingText::DISTANCE = DungeonTilemap::SIZE;

std::map<int, std::vector<FloatingText*>> FloatingText::stacks;

void FloatingText::push(FloatingText* txt, int key)
{
	txt->key = key;

	std::map<int, std::vector<FloatingText*>>::iterator itr = stacks.find(key);
	if (itr == stacks.end())
	{
		stacks.insert(std::make_pair(key, std::vector<FloatingText*>()));
	}

	std::vector<FloatingText*> stack = stacks.find(key)->second;

	if (stack.size() > 0) 
	{
		FloatingText* below = txt;
		int aboveIndex = stack.size() - 1;
		while (aboveIndex >= 0) 
		{
			FloatingText* above = stack[aboveIndex];
			if (above->y + above->Height() > below->y) 
			{
				above->y = below->y - above->Height();

				below = above;
				aboveIndex--;
			}
			else 
			{
				break;
			}
		}
	}

	stack.push_back(txt);
}

FloatingText::FloatingText()
{
	tag = "FloatingText";

	key = -1;
	cameraZoom = -1;

	speed.y = -DISTANCE / LIFESPAN;
}

void FloatingText::update()
{
	BitmapText::update();

	if (timeLeft > 0) 
	{
		if ((timeLeft -= Game::elapsed) <= 0) 
		{
			kill();
		}
		else 
		{
			float p = timeLeft / LIFESPAN;
			alpha(p > 0.5f ? 1 : p * 2);
		}
	}
}

void FloatingText::kill()
{
	if (key != -1) 
	{
		std::vector<FloatingText*>& sv = stacks.find(key)->second;
		for (std::vector<FloatingText*>::iterator itr = sv.begin();
			itr != sv.end(); itr++)
		{
			if (*itr == this)
			{
				sv.erase(itr);
				break;
			}
		}

		key = -1;
	}

	BitmapText::kill();
}

void FloatingText::destroy()
{
	kill();
	BitmapText::destroy();
}

void FloatingText::reset(float x, float y, const std::string& text, int color)
{
	revive();

	if (cameraZoom != Camera::mainCamera->zoomf) 
	{
		cameraZoom = Camera::mainCamera->zoomf;
		PixelScene::chooseFont(9, cameraZoom);
		//font = PixelScene.font;

		GameMath::PointFSet(&scale, PixelScene::scale);
		//scale.set(PixelScene.scale);
	}

	this->text(text);
	hardlight(color);

	measure();
	this->x = PixelScene::align(x - Width() / 2);
	this->y = y - Height();

	timeLeft = LIFESPAN;
}

void FloatingText::show(float x, float y, const std::string& text, int color)
{
	GameScene::status()->reset(x, y, text, color);
}

void FloatingText::show(float x, float y, int key, const std::string& text, int color)
{
	FloatingText* txt = GameScene::status();
	txt->reset(x, y, text, color);
	push(txt, key);
}
