#include "busyindicator.h"
#include "icon.h"
#include "util.h"
#include "dungeon.h"
#include "hero.h"

BusyIndicator::BusyIndicator()
:Image(Icons::get(Icons::BUSY))
{
	GameMath::PointFSet(&origin, width / 2, height / 2);
	//origin.set(width / 2, height / 2);
	angularSpeed = 720;
}

void BusyIndicator::update()
{
	Image::update();
	visible = Dungeon::hero->isAlive() && !Dungeon::hero->ready;
}
