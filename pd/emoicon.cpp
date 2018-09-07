#include "emoicon.h"
#include "gamescene.h"
#include "game.h"
#include "charsprite.h"
#include "icon.h"

EmoIcon::EmoIcon(CharSprite* owner)
{
	this->owner = owner;
	//GameScene::add(this);
}

void EmoIcon::update()
{
	Image::update();

	if (visible) 
	{
		if (growing) 
		{
			GameMath::PointFSet(&scale, scale.x + Game::elapsed * timeScale);
			//scale.set(scale.x + Game.elapsed * timeScale);
			if (scale.x > maxSize) 
			{
				growing = false;
			}
		}
		else 
		{
			GameMath::PointFSet(&scale, scale.x - Game::elapsed * timeScale);
			//scale.set(scale.x - Game.elapsed * timeScale);
			if (scale.x < 1) 
			{
				growing = true;
			}
		}

		x = owner->x + owner->width - width / 2;
		y = owner->y - height;
	}
}

EmoIconSleep::EmoIconSleep(CharSprite* owner)
:EmoIcon(owner)
{
	copy(Icons::get(Icons::SLEEP));

	maxSize = 1.2f;
	timeScale = 0.5f;

	GameMath::PointFSet(&origin, width / 2, height / 2);
	//origin.set(width / 2, height / 2);
	GameMath::PointFSet(&scale, Random::Float(1, maxSize));
	//scale.set(Random.Float(1, maxSize));
}

EmoIconAlert::EmoIconAlert(CharSprite* owner)
:EmoIcon(owner)
{
	copy(Icons::get(Icons::ALERT));

	maxSize = 1.3f;
	timeScale = 2;

	GameMath::PointFSet(&origin, 2.5f, height - 2.5f);
	//origin.set(2.5f, height - 2.5f);
	GameMath::PointFSet(&scale, Random::Float(1, maxSize));
	//scale.set(Random.Float(1, maxSize));
}