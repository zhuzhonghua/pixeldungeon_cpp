#include "door.h"
#include "level.h"
#include "terrain.h"
#include "gamescene.h"
#include "dungeon.h"

void Door::enter(int pos)
{
	Level::set(pos, Terrain::OPEN_DOOR);
	GameScene::updateMap(pos);
	Dungeon::observe();

	if (Dungeon::visible[pos]) {
		//Sample.INSTANCE.play(Assets.SND_OPEN);
	}
}

void Door::leave(int pos)
{
	if (Dungeon::level->heaps.get(pos) == NULL) {
		Level::set(pos, Terrain::DOOR);
		GameScene::updateMap(pos);
		Dungeon::observe();
	}
}
