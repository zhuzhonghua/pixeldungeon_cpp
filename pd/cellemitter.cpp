#include "cellemitter.h"
#include "dungeontilemap.h"
#include "gamescene.h"

Emitter* CellEmitter::get(int cell)
{
	PointF p = DungeonTilemap::tileToWorld(cell);

	Emitter* emitter = GameScene::emitter();
	emitter->pos(p.x, p.y, DungeonTilemap::SIZE, DungeonTilemap::SIZE);

	return emitter;
}

Emitter* CellEmitter::center(int cell)
{
	PointF p = DungeonTilemap::tileToWorld(cell);

	Emitter* emitter = GameScene::emitter();
	emitter->pos(p.x + DungeonTilemap::SIZE / 2, p.y + DungeonTilemap::SIZE / 2);

	return emitter;
}

Emitter* CellEmitter::bottom(int cell) 
{
	PointF p = DungeonTilemap::tileToWorld(cell);

	Emitter* emitter = GameScene::emitter();
	emitter->pos(p.x, p.y + DungeonTilemap::SIZE, DungeonTilemap::SIZE, 0);

	return emitter;
}