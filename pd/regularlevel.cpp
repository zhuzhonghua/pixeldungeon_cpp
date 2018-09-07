#include "regularlevel.h"
#include "util.h"
#include "room.h"
#include "graph.h"
#include "dungeon.h"
#include "terrain.h"
#include "painter.h"
#include "mob.h"
#include "bestiary.h"
#include "heap.h"
#include "generator.h"

RegularLevel::RegularLevel()
{
	secretDoors = 0;

	minRoomSize = 7;
	maxRoomSize = 9;
}

RegularLevel::~RegularLevel()
{
	for (std::set<Room*>::iterator itr = rooms.begin();
		itr != rooms.end(); itr++)
	{
		delete *itr;
	}
	rooms.clear();
}

int RegularLevel::nMobs()
{
	return 2 + Dungeon::depth % 5 + Random::Int(3);
}

void RegularLevel::storeInBundle(Bundle* bundle)
{
	//super.storeInBundle(bundle);
	//bundle.put("rooms", rooms);
}

void RegularLevel::restoreFromBundle(Bundle* bundle)
{
	//super.restoreFromBundle(bundle);
	//
	//rooms = new HashSet<Room>((Collection< ? extends Room>) bundle.getCollection("rooms"));
	//for (Room r : rooms) {
	//	if (r.type == Type.WEAK_FLOOR) {
	//		weakFloorCreated = true;
	//		break;
	//	}
	//}
}

bool RegularLevel::build()
{
	if (!initRooms()) 
	{
		return false;
	}

	std::set<Graph::Node*> tmpRooms(rooms.begin(), rooms.end());

	int distance;
	int retry = 0;
	int minDistance = (int)std::sqrt(rooms.size());
	do {
		do {
			roomEntrance = Random::element(rooms);
		} while (roomEntrance->width() < 4 || roomEntrance->height() < 4);

		do {
			roomExit = Random::element(rooms);
		} while (roomExit == roomEntrance || roomExit->width() < 4 || roomExit->height() < 4);

		Graph::buildDistanceMap(tmpRooms, roomExit);
		distance = roomEntrance->Distance();

		if (retry++ > 10) 
		{
			return false;
		}

	} while (distance < minDistance);

	roomEntrance->type = Room::Type::ENTRANCE;
	roomExit->type = Room::Type::EXIT;

	std::set<Room*> connected;
	connected.insert(roomEntrance);

	Graph::buildDistanceMap(tmpRooms, roomExit);
	std::list<Graph::Node*> path = Graph::buildPath(tmpRooms, roomEntrance, roomExit);

	// Assert
	if (path.size() <= 0)
	{
		*(int*)0 = 0;
	}

	Room* room = roomEntrance;
	for (std::list<Graph::Node*>::iterator itr = path.begin();
		itr != path.end(); itr++)
	{
		Room* next = (Room*)*itr;
		room->connect(next);
		room = next;
		connected.insert(room);
	}

	Graph::setPrice(path, roomEntrance->distance);

	Graph::buildDistanceMap(tmpRooms, roomExit);
	path = Graph::buildPath(tmpRooms, roomEntrance, roomExit);

	// Assert
	if (path.size() <= 0)
	{
		*(int*)0 = 0;
	}

	room = roomEntrance;
	for (std::list<Graph::Node*>::iterator itr = path.begin();
		itr != path.end(); itr++)
	{
		Room* next = (Room*)*itr;
		room->connect(next);
		room = next;
		connected.insert(room);
	}

	int count = 0;
	int nConnected = (int)(rooms.size() * Random::Float(0.5f, 0.7f));
	while (connected.size() < nConnected && count++ < 100000) 
	{
		Room* cr = Random::element(connected);
		Room* or = Random::element(cr->neigbours);
		if (connected.find(or) == connected.end()) 
		{
			cr->connect(or);
			connected.insert(or);
		}
	}

	if (Dungeon::shopOnLevel()) 
	{
		Room* shop = NULL;
		for (std::map<Room*, Room::Door*>::iterator itr = roomEntrance->connected.begin();
			itr != roomEntrance->connected.end(); itr++)
		{
			Room* r = itr->first;
			if (r->connected.size() == 1 && r->width() >= 5 && r->height() >= 5) 
			{
				shop = r;
				break;
			}
		}
	
		if (shop == NULL) 
		{
			return false;
		}
		else 
		{
			shop->type = Room::Type::SHOP;
		}
	}
	
	specials.assign(Room::SPECIALS.begin(), Room::SPECIALS.end());

	if (Dungeon::bossLevel(Dungeon::depth + 1)) 
	{
		specials.remove(Room::Type::WEAK_FLOOR);
	}
	assignRoomType();

	paint();
	paintWater();
	paintGrass();

	placeTraps();

	return true;
}

bool RegularLevel::initRooms()
{
	for (std::set<Room*>::iterator itr = rooms.begin();
		itr != rooms.end(); itr++)
	{
		delete *itr;
	}
	rooms.clear();

	split(GameMath::RECT(0, 0, WIDTH - 1, HEIGHT - 1));

	if (rooms.size() < 8) 
	{
		return false;
	}

	std::vector<Room*> ra(rooms.begin(), rooms.end());
	for (int i = 0; i < ra.size() - 1; i++) 
	{
		for (int j = i + 1; j < ra.size(); j++) 
		{
			ra[i]->addNeigbour(ra[j]);
		}
	}

	return true;
}

void RegularLevel::split(Rect rect)
{
	int w = GameMath::RECTWidth(rect);
	int h = GameMath::RECTHeight(rect);

	int left = GameMath::RECTLeft(rect);
	int right = GameMath::RECTRight(rect);
	int top = GameMath::RECTTop(rect);
	int bottom = GameMath::RECTBottom(rect);

	int square = GameMath::RECTSquare(rect);

	if (w > maxRoomSize && h < minRoomSize) 
	{
		int vw = Random::Int(left + 3, right - 3);
		split(GameMath::RECT(left, top, vw, bottom));
		split(GameMath::RECT(vw, top, right, bottom));
	}
	else if (h > maxRoomSize && w < minRoomSize) 
	{
		int vh = Random::Int(top + 3, bottom - 3);
		split(GameMath::RECT(left, top, right, vh));
		split(GameMath::RECT(left, vh, right, bottom));
	}
	else if ((Random::random() <= (minRoomSize * minRoomSize / square) && w <= maxRoomSize && h <= maxRoomSize) || w < minRoomSize || h < minRoomSize) 
	{
		Room* r = new Room();
		r->set(rect);
		rooms.insert(r);
	}
	else 
	{
		if (Random::Float() < (float)(w - 2) / (w + h - 4)) 
		{
			int vw = Random::Int(left + 3, right - 3);
			split(GameMath::RECT(left, top, vw, bottom));
			split(GameMath::RECT(vw, top, right, bottom));
		}
		else 
		{
			int vh = Random::Int(top + 3, bottom - 3);
			split(GameMath::RECT(left, top, right, vh));
			split(GameMath::RECT(left, vh, right, bottom));
		}
	}
}

void RegularLevel::paint()
{
	for (std::set<Room*>::iterator itr = rooms.begin();
		itr != rooms.end(); itr++)
	{
		Room* r = *itr;
		if (r->type != Room::Type::NONE)
		{
			placeDoors(r);
			Room::paint(r->type, this, r);
		}
		else 
		{
			if (feeling == Feeling::CHASM && Random::Int(2) == 0) {
				Painter::fill(this, r->getBounds(), Terrain::WALL);
			}
		}
	}

	for (std::set<Room*>::iterator itr = rooms.begin();
		itr != rooms.end(); itr++)
	{
		Room* r = *itr;
		paintDoors(r);
	}
}

void RegularLevel::paintWater()
{
	std::vector<bool> lake = water();
	for (int i = 0; i < LENGTH; i++) 
	{
		if (map[i] == Terrain::EMPTY && lake[i]) 
		{
			map[i] = Terrain::WATER;
		}
	}
}

void RegularLevel::paintGrass()
{
	std::vector<bool> gra = grass();

	if (feeling == Feeling::GRASS) 
	{
		for (std::set<Room*>::iterator itr = rooms.begin();
			itr != rooms.end(); itr++)		
		{
			Room* room = *itr;

			Rect bound = room->getBounds();
			int top = GameMath::RECTTop(bound);
			int bottom = GameMath::RECTBottom(bound);
			int left = GameMath::RECTLeft(bound);
			int right = GameMath::RECTRight(bound);

			if (room->type != Room::Type::NONE && room->type != Room::Type::PASSAGE && room->type != Room::Type::TUNNEL) 
			{
				gra[(left + 1) + (top + 1) * WIDTH] = true;
				gra[(right - 1) + (top + 1) * WIDTH] = true;
				gra[(left + 1) + (bottom - 1) * WIDTH] = true;
				gra[(right - 1) + (bottom - 1) * WIDTH] = true;
			}
		}
	}

	for (int i = WIDTH + 1; i < LENGTH - WIDTH - 1; i++) 
	{
		if (map[i] == Terrain::EMPTY && gra[i]) 
		{
			int count = 1;
			for (int j = 0; j < 8/*sizeof(Level::NEIGHBOURS8) / sizeof(int)*/; j++)
			{
				int n = NEIGHBOURS8[j];
				if (gra[i + n])
				{
					count++;
				}
			}
			map[i] = (Random::Float() < count / 12.0f) ? Terrain::HIGH_GRASS : Terrain::GRASS;
		}
	}
}

void RegularLevel::placeTraps()
{
	int nTps = nTraps();
	std::vector<float> trapCha = trapChances();

	for (int i = 0; i < nTps; i++) 
	{
		int trapPos = Random::Int(LENGTH);

		if (map[trapPos] == Terrain::EMPTY) 
		{
			switch (Random::chances(trapCha)) 
			{
			case 0:
				map[trapPos] = Terrain::SECRET_TOXIC_TRAP;
				break;
			case 1:
				map[trapPos] = Terrain::SECRET_FIRE_TRAP;
				break;
			case 2:
				map[trapPos] = Terrain::SECRET_PARALYTIC_TRAP;
				break;
			case 3:
				map[trapPos] = Terrain::SECRET_POISON_TRAP;
				break;
			case 4:
				map[trapPos] = Terrain::SECRET_ALARM_TRAP;
				break;
			case 5:
				map[trapPos] = Terrain::SECRET_LIGHTNING_TRAP;
				break;
			case 6:
				map[trapPos] = Terrain::SECRET_GRIPPING_TRAP;
				break;
			case 7:
				map[trapPos] = Terrain::SECRET_SUMMONING_TRAP;
				break;
			}
		}
	}
}

void RegularLevel::paintDoors(Room* r)
{
	for (std::map<Room*, Room::Door*>::iterator itr = r->connected.begin();
		itr != r->connected.end(); itr++)
	{
		Room* n = itr->first;

		if (joinRooms(r, n)) 
		{
			continue;
		}

		Room::Door* d = itr->second;
		int door = d->getPoint().x + d->getPoint().y * WIDTH;

		switch (d->type) {
		case Room::Door::EMPTY:
			map[door] = Terrain::EMPTY;
			break;
		case Room::Door::TUNNEL:
			map[door] = tunnelTile();
			break;
		case Room::Door::REGULAR:
			if (Dungeon::depth <= 1) 
			{
				map[door] = Terrain::DOOR;
			}
			else 
			{
				bool secret = (Dungeon::depth < 6 ? Random::Int(12 - Dungeon::depth) : Random::Int(6)) == 0;
				map[door] = secret ? Terrain::SECRET_DOOR : Terrain::DOOR;
				if (secret) 
				{
					secretDoors++;
				}
			}
			break;
		case Room::Door::UNLOCKED:
			map[door] = Terrain::DOOR;
			break;
		case Room::Door::HIDDEN:
			map[door] = Terrain::SECRET_DOOR;
			secretDoors++;
			break;
		case Room::Door::BARRICADE:
			map[door] = Random::Int(3) == 0 ? Terrain::BOOKSHELF : Terrain::BARRICADE;
			break;
		case Room::Door::LOCKED:
			map[door] = Terrain::LOCKED_DOOR;
			break;
		}
	}
}

void RegularLevel::assignRoomType()
{
	int specialRooms = 0;

	for (std::set<Room*>::iterator itr = rooms.begin();
		itr != rooms.end(); itr++)
	{
		Room* r = *itr;
		if (r->type == Room::Type::NONE &&
			r->connected.size() == 1) 
		{
			int width = GameMath::RECTWidth(r->getBounds());
			int height = GameMath::RECTHeight(r->getBounds());

			if (specials.size() > 0 &&
				width > 3 && height > 3 &&
				Random::Int(specialRooms * specialRooms + 2) == 0) 
			{
				if (pitRoomNeeded) 
				{
					r->type = Room::Type::PIT;
					pitRoomNeeded = false;

					specials.remove(Room::Type::ARMORY);
					specials.remove(Room::Type::CRYPT);
					specials.remove(Room::Type::LABORATORY);
					specials.remove(Room::Type::LIBRARY);
					specials.remove(Room::Type::STATUE);
					specials.remove(Room::Type::TREASURY);
					specials.remove(Room::Type::VAULT);
					specials.remove(Room::Type::WEAK_FLOOR);
				}
				else if (Dungeon::depth % 5 == 2 && 
						std::find(specials.begin(), specials.end(), Room::Type::LABORATORY) != specials.end()) 
				{
					r->type = Room::Type::LABORATORY;
				}
				else 
				{
					int n = specials.size();
					int idx = std::min(Random::Int(n), Random::Int(n));
					int c = 0;
					for (std::list<Room::Type>::iterator itr = specials.begin();
						itr != specials.end(); itr++)
					{
						if (c == idx)
						{
							r->type = *itr;
							break;
						}
						c++;
					}
					if (r->type == Room::Type::WEAK_FLOOR) 
					{
						weakFloorCreated = true;
					}
				}

				Room::useType(r->type);
				specials.remove(r->type);
				specialRooms++;

			}
			else if (Random::Int(2) == 0)
			{
				std::set<Room*> neigbours;
				for (std::set<Room*>::iterator itr = r->neigbours.begin();
					itr != r->neigbours.end(); itr++)				
				{
					Room* n = *itr;
					if (r->connected.end() == r->connected.find(n) &&
						Room::SPECIALS.end() == std::find(Room::SPECIALS.begin(), Room::SPECIALS.end(), n->type) &&
						n->type != Room::Type::PIT) 
					{
						neigbours.insert(n);
					}
				}
				if (neigbours.size() > 1) 
				{
					r->connect(Random::element(neigbours));
				}
			}
		}
	}

	int count = 0;
	for (std::set<Room*>::iterator itr = rooms.begin();
		itr != rooms.end(); itr++)
	{
		Room* r = *itr;
		if (r->type == Room::Type::NONE)
		{
			int connections = r->connected.size();
			if (connections == 0) 
			{

			}
			else if (Random::Int(connections * connections) == 0) 
			{
				r->type = Room::Type::STANDARD;
				count++;
			}
			else 
			{
				r->type = Room::Type::TUNNEL;
			}
		}
	}

	while (count < 4) 
	{
		Room* r = randomRoom(Room::Type::TUNNEL, 1);
		if (r != NULL) 
		{
			r->type = Room::Type::STANDARD;
			count++;
		}
	}
}

Room* RegularLevel::randomRoom(Room::Type type, int tries)
{
	for (int i = 0; i < tries; i++) 
	{
		Room* room = Random::element(rooms);
		if (room->type == type) 
		{
			return room;
		}
	}
	return NULL;
}

int RegularLevel::randomDropCell()
{
	while (true) 
	{
		Room* room = randomRoom(Room::STANDARD, 1);
		if (room != NULL) 
		{
			int pos = room->random();
			if (passable[pos])
			{
				return pos;
			}
		}
	}
}

Room* RegularLevel::room(int pos)
{
	for (std::set<Room*>::iterator itr = rooms.begin();
		itr != rooms.end(); itr++)
	{
		Room* room = *itr;
		if (room->type != Room::Type::NONE && room->inside(pos)) {
			return room;
		}
	}
	return NULL;
}

bool RegularLevel::joinRooms(Room* r, Room* n)
{
	if (r->type != Room::Type::STANDARD || n->type != Room::Type::STANDARD) 
	{
		return false;
	}

	Rect w = GameMath::RECTIntersect(r->getBounds(), n->getBounds());

	int wleft = GameMath::RECTLeft(w);
	int wright = GameMath::RECTRight(w);
	int wtop = GameMath::RECTTop(w);
	int wbottom = GameMath::RECTBottom(w);
	int wheight = GameMath::RECTHeight(w);
	int wwidth = GameMath::RECTWidth(w);

	int rheight = GameMath::RECTHeight(r->getBounds());
	int rwidth = GameMath::RECTWidth(r->getBounds());
	int nheight = GameMath::RECTHeight(n->getBounds());
	int nwidth = GameMath::RECTWidth(n->getBounds());

	if (wleft == wright) 
	{
		if (wbottom - wtop < 3) 
		{
			return false;
		}

		if (wheight == std::max(rheight, nheight)) 
		{
			return false;
		}

		if (rwidth + nwidth > maxRoomSize) 
		{
			return false;
		}

		wtop += 1;
		wbottom -= 0;

		wright++;

		Painter::fill(this, wleft, wtop, 1, wheight, Terrain::EMPTY);

	}
	else 
	{
		if (wright - wleft < 3) 
		{
			return false;
		}

		if (wwidth == std::max(rwidth, nwidth)) {
			return false;
		}

		if (rheight + nheight > maxRoomSize) 
		{
			return false;
		}

		wleft += 1;
		wright -= 0;

		wbottom++;

		Painter::fill(this, wleft, wtop, wwidth, 1, Terrain::EMPTY);
	}

	return true;
}

int RegularLevel::nTraps()
{
	return Dungeon::depth <= 1 ? 0 : Random::Int(1, rooms.size() + Dungeon::depth);
}

std::vector<float> RegularLevel::trapChances()
{
	float chances[] = { 1, 1, 1, 1, 1, 1, 1, 1 };
	return std::vector<float>(chances, chances+(sizeof(chances)/sizeof(float)));
}

void RegularLevel::createMobs()
{
	int n = nMobs();
	for (int i = 0; i < n; i++) 
	{
		Mob* mob = Bestiary::mob(Dungeon::depth);
		do {
			mob->pos = randomRespawnCell();
		} while (mob->pos == -1);
		mobs.insert(mob);
		Actor::occupyCell(mob);
	}
}

void RegularLevel::createItems()
{
	//int nItems = 3;
	//while (Random::Float() < 0.4f) 
	//{
	//	nItems++;
	//}
	//
	//for (int i = 0; i < nItems; i++) {
	//	Heap::Type type = Heap::HEAP;
	//	switch (Random::Int(20)) 
	//	{
	//	case 0:
	//		type = Heap::SKELETON;
	//		break;
	//	case 1:
	//	case 2:
	//	case 3:
	//	case 4:
	//		type = Heap::CHEST;
	//		break;
	//	case 5:
	//		type = Dungeon::depth > 1 ? Heap::MIMIC : Heap::CHEST;
	//		break;
	//	default:
	//		type = Heap::HEAP;
	//	}
	//	drop(Generator::random(), randomDropCell())->type = type;
	//}
	
	//for (Item item : itemsToSpawn) {
	//	int cell = randomDropCell();
	//	if (item instanceof ScrollOfUpgrade) {
	//		while (map[cell] == Terrain.FIRE_TRAP || map[cell] == Terrain.SECRET_FIRE_TRAP) {
	//			cell = randomDropCell();
	//		}
	//	}
	//	drop(item, cell).type = Heap.Type.HEAP;
	//}

	//Item item = Bones.get();
	//if (item != null) {
	//	drop(item, randomDropCell()).type = Heap.Type.SKELETON;
	//}
}

void RegularLevel::placeDoors(Room* r)
{
	for (std::map<Room*, Room::Door*>::iterator itr = r->connected.begin();
		itr != r->connected.end(); itr++)
	{
		Room* n = itr->first;
		Room::Door* door = itr->second;

		if (door == NULL)
		{
			Rect i = GameMath::RECTIntersect(r->getBounds(), n->getBounds());
			int width = GameMath::RECTWidth(i);

			int top = GameMath::RECTTop(i);
			int bottom = GameMath::RECTBottom(i);
			int left = GameMath::RECTLeft(i);
			int right = GameMath::RECTRight(i);

			if (width == 0)
			{
				door = new Room::Door(left, Random::Int(top+1, bottom));
			}
			else
			{
				door = new Room::Door(Random::Int(left, right), top);
			}

			r->connected[n] = door;
			n->connected[r] = door;
		}
	}
}
