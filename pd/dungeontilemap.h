#pragma once

#include "define.h"
#include "tilemap.h"

class Image;

class DungeonTilemap:public Tilemap{
private:
	static DungeonTilemap* instance;

public:
	static const int SIZE = 16;

	DungeonTilemap();

	static PointF tileToWorld(int pos);
	static PointF tileCenterToWorld(int pos);

	int screenToTile(int x, int y);
	virtual bool overlapsPoint(float x, float y) { return true; }
	virtual bool overlapsScreenPoint(int x, int y) { return true; }

	void discover(int pos, int oldValue);
	static Image* tile(int index);
};