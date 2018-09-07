#include "dungeontilemap.h"
#include "level.h"
#include "util.h"
#include "dungeon.h"
#include "texturefilm.h"
#include "camera.h"
#include "image.h"
#include "group.h"
#include "alphatweener.h"

DungeonTilemap* DungeonTilemap::instance;

DungeonTilemap::DungeonTilemap()
:Tilemap(Dungeon::level->tilesTex(),
	new TextureFilm(Dungeon::level->tilesTex(), SIZE, SIZE))
{
	map(Dungeon::level->map, Level::WIDTH);

	instance = this;
}

PointF DungeonTilemap::tileToWorld(int pos)
{
	PointF p(pos % Level::WIDTH, pos / Level::WIDTH);
	GameMath::PointFScale(&p, SIZE);
	return p;
}

PointF DungeonTilemap::tileCenterToWorld(int pos)
{
	return PointF(
		(pos % Level::WIDTH + 0.5f) * SIZE,
		(pos / Level::WIDTH + 0.5f) * SIZE);
}

int DungeonTilemap::screenToTile(int x, int y)
{
	PointF sc = camera()->screenToCamera(x, y);
	PointF thisP = this->point();
	GameMath::PointFOffSet(&sc, GameMath::PointFNegate(&thisP));
	GameMath::PointFInvScale(&sc, SIZE);

	Point p = GameMath::PointFFloor(&sc);

	return p.x >= 0 && p.x < Level::WIDTH && p.y >= 0 && p.y < Level::HEIGHT ? p.x + p.y * Level::WIDTH : -1;
}

namespace{
	class AlphaTweenerNew :public AlphaTweener{
	public:
		DungeonTilemap* dt;
		Image* tile;
		AlphaTweenerNew(DungeonTilemap* dtp, Image* tp, float alpha, float time) 
			:AlphaTweener(tp, alpha, time),dt(dtp), tile(tp){}
	protected:
		virtual void onComplete() 
		{
			tile->killAndErase();
			dt->killAndErase();
		}
	};
}
void DungeonTilemap::discover(int pos, int oldValue)
{
	Image* ti = tile(oldValue);
	ti->point(tileToWorld(pos));

	// For bright mode
	ti->rm = ti->gm = ti->bm = rm;
	ti->ra = ti->ga = ti->ba = ra;
	parent->add(ti);

	parent->add(new AlphaTweenerNew(this, ti, 0, 0.6f));
}

Image* DungeonTilemap::tile(int index)
{
	Image* img = new Image(instance->texture);
	img->frame(instance->tileset->get(index));
	return img;
}
