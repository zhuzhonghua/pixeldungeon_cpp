#include "attackindicator.h"
#include "dangerindicator.h"
#include "charsprite.h"
#include "pixelscene.h"
#include "dungeon.h"
#include "hero.h"
#include "mob.h"
#include "ninepatch.h"
#include "healthindicator.h"

const float AttackIndicator::ENABLED = 1.0f;
const float AttackIndicator::DISABLED = 0.3f;

AttackIndicator* AttackIndicator::instance;
Mob* AttackIndicator::lastTarget;

void AttackIndicator::checkEnemies()
{
	int heroPos = Dungeon::hero->pos;
	candidates.clear();
	int v = Dungeon::hero->VisibleEnemies();
	for (int i = 0; i < v; i++) 
	{
		Mob* mob = Dungeon::hero->visibleEnemy(i);
		if (Level::adjacent(heroPos, mob->pos)) 
		{
			candidates.push_back(mob);
		}
	}

	if (candidates.end() == std::find(candidates.begin(), candidates.end(), lastTarget)) 
	{
		if (candidates.empty()) 
		{
			lastTarget = NULL;
		}
		else 
		{
			int i = Random::Int(candidates.size());
			int c = 0;

			for (std::list<Mob*>::iterator itr = candidates.begin();
				itr != candidates.end(); itr++)
			{
				if (c == i)
				{
					lastTarget = *itr;
					break;
				}
				c++;
			}
			
			updateImage();
			flash();
		}
	}
	else {
		if (!bg->visible) 
		{
			flash();
		}
	}

	visible(lastTarget != NULL);
	enable(bg->visible);
}

void AttackIndicator::updateImage()
{
	if (sprite != NULL) 
	{
		sprite->killAndErase();
		sprite = NULL;
	}

	sprite = lastTarget->Sprite();
	sprite->Idle();
	sprite->paused = true;
	add(sprite);

	sprite->x = _x + (_width - sprite->Width()) / 2 + 1;
	sprite->y = _y + (_height - sprite->Height()) / 2;
	PixelScene::align(sprite);
}

void AttackIndicator::enable(bool value)
{
	enabled = value;
	if (sprite != NULL) 
	{
		sprite->alpha(value ? ENABLED : DISABLED);
	}
}

void AttackIndicator::visible(bool value)
{
	bg->visible = value;
	if (sprite != NULL) 
	{
		sprite->visible = value;
	}
}

AttackIndicator::AttackIndicator()
:Tag(DangerIndicator::COLOR)
{
	sprite = NULL;
	enabled = true;
	// own createchildren do nothing
	//init();

	instance = this;
	setSize(24, 24);
	visible(false);
	enable(false);
}

void AttackIndicator::update()
{
	Tag::update();

	if (Dungeon::hero->isAlive()) 
	{
		if (!Dungeon::hero->ready) 
		{
			enable(false);
		}
	}
	else 
	{
		visible(false);
		enable(false);
	}
}

void AttackIndicator::target(Char* target)
{
	lastTarget = (Mob*)target;
	instance->updateImage();

	HealthIndicator::instance->Target(target);
}

void AttackIndicator::updateState()
{
	instance->checkEnemies();
}

void AttackIndicator::layout()
{
	Tag::layout();

	if (sprite != NULL) 
	{
		sprite->x = _x + (_width - sprite->Width()) / 2;
		sprite->y = _y + (_height - sprite->Height()) / 2;
		PixelScene::align(sprite);
	}
}

void AttackIndicator::onClick()
{
	if (enabled) 
	{
		Dungeon::hero->handle(lastTarget->pos);
	}
}

