#include "room.h"
#include "util.h"
#include "bundle.h"
#include "level.h"
#include "standardpainter.h"

const Room::Type Room::_SPECIALS[] = { 
	Room::Type::ARMORY, 
	Room::Type::WEAK_FLOOR, 
	Room::Type::MAGIC_WELL, 
	Room::Type::CRYPT, 
	Room::Type::POOL, 
	Room::Type::GARDEN, 
	Room::Type::LIBRARY,
	Room::Type::TREASURY, 
	Room::Type::TRAPS, 
	Room::Type::STORAGE, 
	Room::Type::STATUE, 
	Room::Type::LABORATORY, 
	Room::Type::VAULT, 
	Room::Type::ALTAR };

std::list<Room::Type> Room::SPECIALS(_SPECIALS, _SPECIALS + sizeof(_SPECIALS) / sizeof(Room::Type));

std::map<Room::Type, Room::PaintMethod> Room::paintMethods;

Room::Room()
{
	type = Type::NONE;
	price = 1;
}

void Room::set(int left, int top, int right, int bottom)
{
	bounds = GameMath::RECT(left,top,right,bottom);
}

void Room::set(Rect b)
{
	bounds = b;
}

void Room::addNeigbour(Room* other)
{
	Rect i = GameMath::RECTIntersect(bounds, other->bounds);
	int w = GameMath::RECTWidth(i);
	int h = GameMath::RECTHeight(i);

	if ((w == 0 && h >= 3) || 
		(h == 0 && w >= 3)) 
	{
		neigbours.insert(other);
		other->neigbours.insert(this);
	}
}

int Room::width()
{
	return GameMath::RECTWidth(bounds);
}

int Room::height()
{
	return GameMath::RECTHeight(bounds);
}

void Room::storeInBundle(Bundle* bundle)
{
	int left = GameMath::RECTLeft(bounds);
	int right = GameMath::RECTRight(bounds);
	int top = GameMath::RECTTop(bounds);
	int bottom = GameMath::RECTBottom(bounds);

	bundle->put("left", left);
	bundle->put("top", top);
	bundle->put("right", right);
	bundle->put("bottom", bottom);
	//bundle->put("type", type.toString());
}

void Room::restoreFromBundle(Bundle* bundle)
{
	int left = bundle->getInt("left");
	int top = bundle->getInt("top");
	int right = bundle->getInt("right");
	int bottom = bundle->getInt("bottom");
	//type = Type.valueOf(bundle.getString("type"));

	bounds = GameMath::RECT(left, top, right, bottom);
}

void Room::connect(Room* room)
{
	if (connected.find(room) == connected.end())
	{
		connected.insert(std::make_pair(room, (Door*)NULL));
	}
}

int Room::random(int m)
{
	int left = GameMath::RECTLeft(bounds);
	int right = GameMath::RECTRight(bounds);
	int top = GameMath::RECTTop(bounds);
	int bottom = GameMath::RECTBottom(bounds);

	int x = Random::Int(left + 1 + m, right - m);
	int y = Random::Int(top + 1 + m, bottom - m);
	return x + y * Level::WIDTH;
}

void Room::useType(Type type)
{
	if (std::find(SPECIALS.begin(), SPECIALS.end(), type) != SPECIALS.end())
	{
		SPECIALS.remove(type);
		SPECIALS.push_back(type);
	}
}

void Room::paint(Type type, Level* level, Room* r)
{
	PaintMethod pm = getPaintMethod(type);
	if (pm)	pm(level, r);
}

Room::PaintMethod Room::getPaintMethod(Type type)
{
	if (paintMethods.size() == 0)
	{
		paintMethods.insert(std::make_pair(STANDARD, StandardPainter::paint));
		paintMethods.insert(std::make_pair(ENTRANCE, EntrancePainter::paint));
		paintMethods.insert(std::make_pair(EXIT, ExitPainter::paint));
		paintMethods.insert(std::make_pair(BOSS_EXIT, BossExitPainter::paint));
		paintMethods.insert(std::make_pair(TUNNEL, TunnelPainter::paint));
		paintMethods.insert(std::make_pair(PASSAGE, PassagePainter::paint));
		//paintMethods.insert(std::make_pair(SHOP, ShopPainter::paint));
		//paintMethods.insert(std::make_pair(BLACKSMITH, BlacksmithPainter::paint));
		//paintMethods.insert(std::make_pair(TREASURY, TreasuryPainter::paint));
		//paintMethods.insert(std::make_pair(ARMORY, ArmoryPainter::paint));
		//paintMethods.insert(std::make_pair(LIBRARY, LibraryPainter::paint));
		//paintMethods.insert(std::make_pair(LABORATORY, LaboratoryPainter::paint));
		//paintMethods.insert(std::make_pair(VAULT, VaultPainter::paint));
		//paintMethods.insert(std::make_pair(TRAPS, TrapsPainter::paint));
		//paintMethods.insert(std::make_pair(STORAGE, StoragePainter::paint));
		//paintMethods.insert(std::make_pair(MAGIC_WELL, MagicWellPainter::paint));
		//paintMethods.insert(std::make_pair(GARDEN, GardenPainter::paint));
		//paintMethods.insert(std::make_pair(CRYPT, CryptPainter::paint));
		//paintMethods.insert(std::make_pair(STATUE, StatuePainter::paint));
		//paintMethods.insert(std::make_pair(POOL, PoolPainter::paint));
		//paintMethods.insert(std::make_pair(RAT_KING, RatKingPainter::paint));
		paintMethods.insert(std::make_pair(WEAK_FLOOR, WeakFloorPainter::paint));
		//paintMethods.insert(std::make_pair(PIT, PitPainter::paint));
		//paintMethods.insert(std::make_pair(ALTAR, AltarPainter::paint));
	}
	return paintMethods[type];
}

Point Room::center()
{
	int left = GameMath::RECTLeft(bounds);
	int right = GameMath::RECTRight(bounds);
	int top = GameMath::RECTTop(bounds);
	int bottom = GameMath::RECTBottom(bounds);

	return Point(
		(left + right) / 2 + (((right - left) & 1) == 1 ? Random::Int(2) : 0),
		(top + bottom) / 2 + (((bottom - top) & 1) == 1 ? Random::Int(2) : 0));
}

Room::Door* Room::entrance()
{
	return connected.begin()->second;
}

void Room::shuffleTypes()
{
	int size = SPECIALS.size();
	for (int i = 0; i < size - 1; i++) 
	{
		int j = Random::Int(i, size);
		if (j != i) 
		{
			std::list<Type>::iterator iItr;
			std::list<Type>::iterator jItr;
			int count = 0;
			for (std::list<Type>::iterator itr = SPECIALS.begin(); itr != SPECIALS.end(); itr++, count++)
			{
				if (count == i)
				{
					iItr = itr;
				}
				else if (count == j)
				{
					jItr = itr;
				}
			}
			Type t = *iItr;
			//SPECIALS.set(i, SPECIALS.get(j));
			*iItr = *jItr;
			//SPECIALS.set(j, t);
			*jItr = t;
		}
	}
}

bool Room::inside(int p)
{
	int left = GameMath::RECTLeft(bounds);
	int top = GameMath::RECTTop(bounds);
	int right = GameMath::RECTRight(bounds);
	int bottom = GameMath::RECTBottom(bounds);

	int x = p % Level::WIDTH;
	int y = p / Level::WIDTH;
	return x > left && y > top && x < right && y < bottom;
}

Room::Door::Door(int x, int y)
{
	type = Type::EMPTY;
	p.x = x;
	p.y = y;
}

void Room::Door::set(Type type)
{
	if (type - this->type > 0)
	{
		this->type = type;
	}
}
