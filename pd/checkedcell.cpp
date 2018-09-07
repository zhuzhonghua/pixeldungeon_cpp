#include "checkedcell.h"
#include "texturecache.h"
#include "util.h"
#include "dungeontilemap.h"
#include "game.h"

CheckedCell::CheckedCell(int pos)
:Image(TextureCache::createSolid(0xFF55AAFF))
{
	GameMath::PointFSet(&origin, 0.5f);
	//origin.set(0.5f);

	PointF p = DungeonTilemap::tileToWorld(pos);
	GameMath::PointFOffSet(&p, DungeonTilemap::SIZE / 2, DungeonTilemap::SIZE / 2);

	point(p);

	alpha = 0.8f;
}

void CheckedCell::update()
{
	if ((alpha -= Game::elapsed) > 0) 
	{
		Image::alpha(alpha);
		GameMath::PointFSet(&scale, DungeonTilemap::SIZE * alpha);
		//scale.set(DungeonTilemap.SIZE * alpha);
	}
	else 
	{
		killAndErase();
		delete this;
	}
}
