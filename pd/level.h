#pragma once

#include "bundlable.h"
#include "plant.h"
#include <vector>
#include <map>
#include <set>
#include "typedefine.h"

class Scene;
class Char;
class Blob;
class Mob;
class Heap;
class Item;

class Level :public Bundlable{
public:
	enum Feeling {
		NONE,
		CHASM,
		WATER,
		GRASS
	};

	static const int WIDTH = 32;
	static const int HEIGHT = 32;
	static const int LENGTH = WIDTH * HEIGHT;

	static const int NEIGHBOURS4[];
	static const int NEIGHBOURS8[];
	static const int NEIGHBOURS9[];

protected:
	static const float TIME_TO_RESPAWN;

	static bool pitRoomNeeded;
	static bool weakFloorCreated;

private:
	static const std::string TXT_HIDDEN_PLATE_CLICKS;

	static const std::string MAP;
	static const std::string VISITED;
	static const std::string MAPPED;
	static const std::string ENTRANCE;
	static const std::string EXIT;
	static const std::string HEAPS;
	static const std::string PLANTS;
	static const std::string MOBS;
	static const std::string BLOBS;

	void buildFlagMaps();
	int getWaterTile(int pos);
	void cleanWalls();
public:
	static bool resizingNeeded;
	static int loadedMapSize;

	std::vector<int> map;
	std::vector<bool> visited;
	std::vector<bool> mapped;

	static std::vector<bool> fieldOfView;

	static std::vector<bool> passable;
	static std::vector<bool> losBlocking;
	static std::vector<bool> flamable;
	static std::vector<bool> secret;
	static std::vector<bool> solid;
	static std::vector<bool> avoid;
	static std::vector<bool> water;
	static std::vector<bool> pit;

	static std::vector<bool> discoverable;

	int viewDistance;

	Feeling feeling;

	int entrance;
	int exit;

	HashSet<Mob*> mobs;
	HashMap<std::string, Blob*> blobs;
	HashMap<int, Heap*> heaps;
	HashMap<int, Plant*> plants;

	int color1;
	int color2;

	Level();
	virtual ~Level();
	void create();
	void clear();

	int pitCell();
	int randomRespawnCell();
	int adjustPos(int pos);

	virtual std::string tilesTex() { return ""; }
	virtual std::string waterTex() { return ""; }

	void reset();

	std::vector<bool> updateFieldOfView(Char* c);

	static int distance(int a, int b);

	int tunnelTile();

	int randomDestination();
	static bool adjacent(int a, int b);
	void mobPress(Mob* mob);

	static void set(int cell, int terrain);
	Heap* drop(Item* item, int cell);
	void uproot(int pos);
	Plant* plant(Plant::Seed* seed, int pos);
	void press(int cell, Char* ch);
	void destroy(int pos);
	String tileName(int tile);
	String tileDesc(int tile);
protected:
	virtual bool build() = 0;
	virtual void decorate() = 0;
	virtual void createMobs() = 0;
	virtual void createItems() = 0;

public:
	void addVisuals(Scene* scene);
	virtual int nMobs() { return 0; }
};