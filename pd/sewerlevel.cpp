#include "sewerlevel.h"
#include "define.h"
#include "util.h"
#include "terrain.h"
#include "room.h"

std::string SewerLevel::tilesTex()
{
	return Assets::TILES_SEWERS;
}

std::string SewerLevel::waterTex()
{
	return Assets::WATER_SEWERS;
}

std::vector<bool> SewerLevel::water()
{
	return Patch::generate(feeling == Feeling::WATER ? 0.60f : 0.45f, 5);
}

std::vector<bool> SewerLevel::grass()
{
	return Patch::generate(feeling == Feeling::GRASS ? 0.60f : 0.40f, 4);
}

void SewerLevel::decorate()
{
	for (int i = 0; i < WIDTH; i++) {
		if (map[i] == Terrain::WALL &&
			map[i + WIDTH] == Terrain::WATER &&
			Random::Int(4) == 0) {
			map[i] = Terrain::WALL_DECO;
		}
	}

	for (int i = WIDTH; i < LENGTH - WIDTH; i++) {
		if (map[i] == Terrain::WALL &&
			map[i - WIDTH] == Terrain::WALL &&
			map[i + WIDTH] == Terrain::WATER &&
			Random::Int(2) == 0) {

			map[i] = Terrain::WALL_DECO;
		}
	}

	for (int i = WIDTH + 1; i < LENGTH - WIDTH - 1; i++) {
		if (map[i] == Terrain::EMPTY) {

			int count =
				(map[i + 1] == Terrain::WALL ? 1 : 0) +
				(map[i - 1] == Terrain::WALL ? 1 : 0) +
				(map[i + WIDTH] == Terrain::WALL ? 1 : 0) +
				(map[i - WIDTH] == Terrain::WALL ? 1 : 0);

			if (Random::Int(16) < count * count) {
				map[i] = Terrain::EMPTY_DECO;
			}
		}
	}

	while (true) {
		int pos = roomEntrance->random();
		if (pos != entrance) {
			map[pos] = Terrain::SIGN;
			break;
		}
	}
}

void SewerLevel::createMobs()
{
	RegularLevel::createMobs();

	//Ghost.Quest.spawn(this);
}

void SewerLevel::createItems()
{
	//if (Dungeon.dewVial && Random.Int(4 - Dungeon.depth) == 0) {
	//	addItemToSpawn(new DewVial());
	//	Dungeon.dewVial = false;
	//}

	RegularLevel::createItems();
}
