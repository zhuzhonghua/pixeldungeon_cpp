#include "painter.h"
#include "level.h"
#include "util.h"
#include "room.h"
#include "terrain.h"
#include <algorithm>

void Painter::set(Level* level, int cell, int value)
{
	level->map[cell] = value;
}

void Painter::set(Level* level, int x, int y, int value)
{
	set(level, x + y * Level::WIDTH, value);
}

void Painter::set(Level* level, Point p, int value)
{
	set(level, p.x, p.y, value);
}

void Painter::fill(Level* level, int x, int y, int w, int h, int value)
{
	int width = Level::WIDTH;

	int pos = y * width + x;
	for (int i = y; i < y + h; i++, pos += width) {
		Arrays<int>::fill(level->map, pos, pos + w, value);
	}
}

void Painter::fill(Level* level, Rect rect, int value)
{
	int left = GameMath::RECTLeft(rect);
	int top = GameMath::RECTTop(rect);
	int width = GameMath::RECTWidth(rect);
	int height = GameMath::RECTHeight(rect);

	fill(level, left, top, width + 1, height + 1, value);
}

void Painter::fill(Level* level, Rect rect, int m, int value)
{
	int left = GameMath::RECTLeft(rect);
	int top = GameMath::RECTTop(rect);
	int width = GameMath::RECTWidth(rect);
	int height = GameMath::RECTHeight(rect);

	fill(level, left + m, top + m, width + 1 - m * 2, height + 1 - m * 2, value);
}

void Painter::fill(Level* level, Rect rect, int l, int t, int r, int b, int value)
{
	int left = GameMath::RECTLeft(rect);
	int top = GameMath::RECTTop(rect);
	int width = GameMath::RECTWidth(rect);
	int height = GameMath::RECTHeight(rect);

	fill(level, left + l, top + t, width + 1 - (l + r), height + 1 - (t + b), value);
}

Point Painter::drawInside(Level* level, Room* room, Point from, int n, int value)
{
	int left = GameMath::RECTLeft(room->getBounds());
	int top = GameMath::RECTTop(room->getBounds());
	int right = GameMath::RECTRight(room->getBounds());
	int bottom = GameMath::RECTBottom(room->getBounds());

	Point step;
	if (from.x == left) {
		//step.set(+1, 0);
		step.x = 1;
		step.y = 0;
	}
	else if (from.x == right) {
		//step.set(-1, 0);
		step.x = -1;
		step.y = 0;
	}
	else if (from.y == top) {
		//step.set(0, +1);
		step.x = 0;
		step.y = 1;
	}
	else if (from.y == bottom) {
		//step.set(0, -1);
		step.x = 0;
		step.y = -1;
	}

	Point p = from;
	p.x += step.x;
	p.y += step.y;
	for (int i = 0; i < n; i++) 
	{
		if (value != -1) 
		{
			set(level, p, value);
		}
		//p.offset(step);
		p.x += step.x;
		p.y += step.y;
	}

	return p;
}

void EntrancePainter::paint(Level* level, Room* room)
{
	fill(level, room->getBounds(), Terrain::WALL);
	fill(level, room->getBounds(), 1, Terrain::EMPTY);

	for (std::map<Room*, Room::Door*>::iterator itr = room->connected.begin();
		itr != room->connected.end(); itr++)
	{
		itr->second->set(Room::Door::Type::REGULAR);
	}

	level->entrance = room->random(1);
	set(level, level->entrance, Terrain::ENTRANCE);
}

void ExitPainter::paint(Level* level, Room* room)
{
	fill(level, room->getBounds(), Terrain::WALL);
	fill(level, room->getBounds(), 1, Terrain::EMPTY);

	for (std::map<Room*, Room::Door*>::iterator itr = room->connected.begin();
		itr != room->connected.end(); itr++)
	{
		itr->second->set(Room::Door::Type::REGULAR);
	}

	level->exit = room->random(1);
	set(level, level->exit, Terrain::EXIT);
}

void BossExitPainter::paint(Level* level, Room* room)
{
	Rect bound = room->getBounds();
	int top = GameMath::RECTTop(bound);
	int left = GameMath::RECTLeft(bound);
	int right = GameMath::RECTRight(bound);

	fill(level, bound, Terrain::WALL);
	fill(level, bound, 1, Terrain::EMPTY);

	for (std::map<Room*, Room::Door*>::iterator itr = room->connected.begin();
		itr != room->connected.end(); itr++)
	{
		Room::Door* door = itr->second;
		door->set(Room::Door::Type::REGULAR);
	}

	level->exit = top * Level::WIDTH + (left + right) / 2;
	set(level, level->exit, Terrain::LOCKED_EXIT);
}

void TunnelPainter::paint(Level* level, Room* room)
{
	Rect bound = room->getBounds();
	int top = GameMath::RECTTop(bound);
	int bottom = GameMath::RECTBottom(bound);
	int left = GameMath::RECTLeft(bound);
	int right = GameMath::RECTRight(bound);

	int floor = level->tunnelTile();

	Point c = room->center();

	if (room->width() > room->height() || (room->width() == room->height() && Random::Int(2) == 0)) 
	{
		int from = right - 1;
		int to = left + 1;

		for (std::map<Room*, Room::Door*>::iterator itr = room->connected.begin();
			itr != room->connected.end(); itr++)
		{
			Room::Door* door = itr->second;

			Point p = door->getPoint();
			int dx = p.x;
			int dy = p.y;

			int step = dy < c.y ? +1 : -1;

			if (dx == left) 
			{
				from = left + 1;
				for (int i = dy; i != c.y; i += step) 
				{
					set(level, from, i, floor);
				}
			}
			else if (dx == right) 
			{
				to = right - 1;
				for (int i = dy; i != c.y; i += step) 
				{
					set(level, to, i, floor);
				}
			}
			else 
			{
				if (dx < from) 
				{
					from = dx;
				}
				if (dx > to) 
				{
					to = dx;
				}

				for (int i = dy + step; i != c.y; i += step) 
				{
					set(level, dx, i, floor);
				}
			}
		}

		for (int i = from; i <= to; i++) 
		{
			set(level, i, c.y, floor);
		}
	}
	else 
	{
		int from = bottom - 1;
		int to = top + 1;

		for (std::map<Room*, Room::Door*>::iterator itr = room->connected.begin();
			itr != room->connected.end(); itr++)
		{
			Room::Door* door = itr->second;
			Point p = door->getPoint();
			int dx = p.x;
			int dy = p.y;

			int step = dx < c.x ? +1 : -1;

			if (dy == top) {

				from = top + 1;
				for (int i = dx; i != c.x; i += step) 
				{
					set(level, i, from, floor);
				}

			}
			else if (dy == bottom) 
			{
				to = bottom - 1;
				for (int i = dx; i != c.x; i += step) 
				{
					set(level, i, to, floor);
				}

			}
			else 
			{
				if (dy < from) 
				{
					from = dy;
				}
				if (dy > to) 
				{
					to = dy;
				}

				for (int i = dx + step; i != c.x; i += step) 
				{
					set(level, i, dy, floor);
				}
			}
		}

		for (int i = from; i <= to; i++) 
		{
			set(level, c.x, i, floor);
		}
	}

	for (std::map<Room*, Room::Door*>::iterator itr = room->connected.begin();
		itr != room->connected.end(); itr++)
	{
		Room::Door* door = itr->second;
		door->set(Room::Door::Type::TUNNEL);
	}
}

int PassagePainter::pasWidth;

int PassagePainter::pasHeight;

int PassagePainter::xy2p(Room* room, Point xy)
{
	Rect bound = room->getBounds();
	int top = GameMath::RECTTop(bound);
	int bottom = GameMath::RECTBottom(bound);
	int left = GameMath::RECTLeft(bound);
	int right = GameMath::RECTRight(bound);

	if (xy.y == top) 
	{
		return (xy.x - left - 1);
	}
	else if (xy.x == right)
	{
		return (xy.y - top - 1) + pasWidth;
	}
	else if (xy.y == bottom) 
	{
		return (right - xy.x - 1) + pasWidth + pasHeight;
	}
	else /*if (xy.x == room.left)*/ 
	{
		if (xy.y == top + 1) 
		{
			return 0;
		}
		else 
		{
			return (bottom - xy.y - 1) + pasWidth * 2 + pasHeight;
		}
	}
}

Point PassagePainter::p2xy(Room* room, int p)
{
	Rect bound = room->getBounds();
	int top = GameMath::RECTTop(bound);
	int bottom = GameMath::RECTBottom(bound);
	int left = GameMath::RECTLeft(bound);
	int right = GameMath::RECTRight(bound);

	if (p < pasWidth) 
	{
		return Point(left + 1 + p, top + 1);
	}
	else if (p < pasWidth + pasHeight)
	{
		return Point(right - 1, top + 1 + (p - pasWidth));
	}
	else if (p < pasWidth * 2 + pasHeight) 
	{
		return Point(right - 1 - (p - (pasWidth + pasHeight)), bottom - 1);
	}
	else 
	{
		return Point(left + 1, bottom - 1 - (p - (pasWidth * 2 + pasHeight)));
	}
}

void PassagePainter::paint(Level* level, Room* room)
{
	pasWidth = room->width() - 2;
	pasHeight = room->height() - 2;

	int floor = level->tunnelTile();

	std::vector<int> joints;
	for (std::map<Room*, Room::Door*>::iterator itr = room->connected.begin();
		itr != room->connected.end(); itr++)
	{
		Room::Door* door = itr->second;
		joints.push_back(xy2p(room, door->getPoint()));
	}
	std::sort(joints.begin(), joints.end());

	int nJoints = joints.size();
	int perimeter = pasWidth * 2 + pasHeight * 2;

	int start = 0;
	int maxD = joints[0] + perimeter - joints[nJoints - 1];
	for (int i = 1; i < nJoints; i++) 
	{
		int d = joints[i] - joints[i - 1];
		if (d > maxD) 
		{
			maxD = d;
			start = i;
		}
	}

	int end = (start + nJoints - 1) % nJoints;

	int p = joints[start];
	do {
		set(level, p2xy(room, p), floor);
		p = (p + 1) % perimeter;
	} while (p != joints[end]);

	set(level, p2xy(room, p), floor);

	for (std::map<Room*, Room::Door*>::iterator itr = room->connected.begin();
		itr != room->connected.end(); itr++)
	{
		Room::Door* door = itr->second;
		door->set(Room::Door::Type::TUNNEL);
	}
}

void BlacksmithPainter::paint(Level* level, Room* room)
{
	//fill(level, room, Terrain.WALL);
	//fill(level, room, 1, Terrain.FIRE_TRAP);
	//fill(level, room, 2, Terrain.EMPTY_SP);
	//
	//for (int i = 0; i < 2; i++) {
	//	int pos;
	//	do {
	//		pos = room.random();
	//	} while (level.map[pos] != Terrain.EMPTY_SP);
	//	level.drop(
	//		Generator.random(Random.oneOf(
	//		Generator.Category.ARMOR,
	//		Generator.Category.WEAPON
	//		)), pos);
	//}
	//
	//for (Room.Door door : room.connected.values()) {
	//	door.set(Room.Door.Type.UNLOCKED);
	//	drawInside(level, room, door, 1, Terrain.EMPTY);
	//}
	//
	//Blacksmith npc = new Blacksmith();
	//do {
	//	npc.pos = room.random(1);
	//} while (level.heaps.get(npc.pos) != null);
	//level.mobs.add(npc);
	//Actor.occupyCell(npc);
}

void WeakFloorPainter::paint(Level* level, Room* room)
{
	Rect bound = room->getBounds();
	int top = GameMath::RECTTop(bound);
	int bottom = GameMath::RECTBottom(bound);
	int left = GameMath::RECTLeft(bound);
	int right = GameMath::RECTRight(bound);

	fill(level, bound, Terrain::WALL);
	fill(level, bound, 1, Terrain::CHASM);

	Room::Door* door = room->entrance();
	door->set(Room::Door::Type::REGULAR);

	Point p = door->getPoint();
	int dx = p.x;
	int dy = p.y;

	if (dx == left) 
	{
		for (int i = top + 1; i < bottom; i++) 
		{
			drawInside(level, room, Point(left, i), Random::IntRange(1, room->width() - 2), Terrain::EMPTY_SP);
		}
	}
	else if (dx == right) 
	{
		for (int i = top + 1; i < bottom; i++) 
		{
			drawInside(level, room, Point(right, i), Random::IntRange(1, room->width() - 2), Terrain::EMPTY_SP);
		}
	}
	else if (dy == top) 
	{
		for (int i = left + 1; i < right; i++) 
		{
			drawInside(level, room, Point(i, top), Random::IntRange(1, room->height() - 2), Terrain::EMPTY_SP);
		}
	}
	else if (dy == bottom) 
	{
		for (int i = left + 1; i < right; i++) 
		{
			drawInside(level, room, Point(i, bottom), Random::IntRange(1, room->height() - 2), Terrain::EMPTY_SP);
		}
	}
}
