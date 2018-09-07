#include "dangerindicator.h"
#include "pixelscene.h"
#include "icon.h"
#include "dungeon.h"
#include "healthindicator.h"
#include "mob.h"
#include "charsprite.h"
#include "hero.h"

void DangerIndicator::placeNumber()
{
	number->x = right() - 11 - number->Width();
	number->y = PixelScene::align(_y + (_height - number->baseLine()) / 2);
}

DangerIndicator::DangerIndicator()
:Tag(0xFF4C4C)
{
	enemyIndex = 0;
	lastNumber = -1;

	init();

	setSize(24, 16);

	visible = false;
}

void DangerIndicator::update()
{
	if (Dungeon::hero->isAlive()) 
	{
		int v = Dungeon::hero->VisibleEnemies();
		if (v != lastNumber) 
		{
			lastNumber = v;
			if (visible = lastNumber > 0) 
			{
				number->text(GameMath::format("%d",lastNumber));
				number->measure();
				placeNumber();

				flash();
			}
		}
	}
	else 
	{
		visible = false;
	}

	Tag::update();
}

void DangerIndicator::createChildren()
{
	number = PixelScene::createText(9);
	add(number);

	icon = Icons::get(Icons::SKULL);
	add(icon);
}

void DangerIndicator::layout()
{
	Tag::layout();

	icon->x = right() - 10;
	icon->y = _y + (_height - icon->height) / 2;

	placeNumber();
}

void DangerIndicator::onClick()
{
	Mob* target = Dungeon::hero->visibleEnemy(enemyIndex++);
	//
	HealthIndicator::instance->Target((Char*)target == HealthIndicator::instance->Target() ? NULL : target);

	Camera::mainCamera->target = NULL;
	Camera::mainCamera->focusOn(target->sprite);
}
