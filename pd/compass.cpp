#include "compass.h"
#include "icon.h"
#include "util.h"
#include "dungeontilemap.h"
#include "dungeon.h"
#include "level.h"
#include "camera.h"

const float Compass::RAD_2_G = 180.0f / 3.1415926f;
const float Compass::RADIUS = 12;

Compass::Compass(int cell)
{
	Image* c = Icons::get(Icons::COMPASS);
	copy(*c);
	GameMath::PointFSet(&origin, width / 2, RADIUS);
	//origin.set(width / 2, RADIUS);

	this->cell = cell;
	cellCenter = DungeonTilemap::tileCenterToWorld(cell);
	visible = false;

	delete c;
}

void Compass::update()
{
	Image::update();

	if (!visible) 
	{
		visible = Dungeon::level->visited[cell] || Dungeon::level->mapped[cell];
	}

	if (visible) 
	{
		PointF scroll = Camera::mainCamera->scroll;
		if (!GameMath::PointFEqual(scroll, lastScroll))
		{
			lastScroll = scroll;
			//lastScroll.set(scroll);
			PointF center = Camera::mainCamera->center();
			
			GameMath::PointFOffSet(&center, &scroll);
			angle = (float)std::atan2(cellCenter.x - center.x, center.y - cellCenter.y) * RAD_2_G;
		}
	}
}
