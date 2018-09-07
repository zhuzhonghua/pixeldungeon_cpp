#pragma once

#include "bundlable.h"
#include "define.h"
#include "graph.h"

#include <set>
#include <map>

class Level;
class Room:public Bundlable,public Graph::Node{
public:
	enum Type{
		NONE,
		STANDARD,
		ENTRANCE,
		EXIT,
		BOSS_EXIT,
		TUNNEL,
		PASSAGE,
		SHOP,
		BLACKSMITH,
		TREASURY,
		ARMORY,
		LIBRARY,
		LABORATORY,
		VAULT,
		TRAPS,
		STORAGE,
		MAGIC_WELL,
		GARDEN,
		CRYPT,
		STATUE,
		POOL,
		RAT_KING,
		WEAK_FLOOR,
		PIT,
		ALTAR,
		ALL
	};

	class Door{
	private:
		Point p;
	public:
		enum Type {
			EMPTY, TUNNEL, REGULAR, UNLOCKED, HIDDEN, BARRICADE, LOCKED
		};
		Type type;
		Door(int x, int y);
		void set(Type type);
		Point getPoint() { return p; }
	};

	typedef void(*PaintMethod)(Level* l, Room* r);
	
private:
	Rect bounds;

	static const Type _SPECIALS[];
	static std::map<Type, PaintMethod> paintMethods;
public:
	std::set<Room*> neigbours;
	std::map<Room*, Door*> connected;	
	static std::list<Type> SPECIALS;

	int distance;
	int price;
	Type type;

	Room();

	void set(int left, int top, int right, int bottom);
	void set(Rect b);
	void addNeigbour(Room* other);
	int width();
	int height();
	Point center();
	void connect(Room* room);
	int random() { return random(0); }
	int random(int m);
	Rect getBounds() { return bounds; }
	Door* entrance();
	bool inside(int p);

	static void useType(Type type);
	static void paint(Type type, Level* level, Room* r);
	static PaintMethod getPaintMethod(Type type);

	virtual int Distance() { return distance; }
	virtual void Distance(int v) { distance = v; }
	virtual int Price() { return price; }
	virtual void Price(int v) { price = v; }
	virtual std::set<Graph::Node*> edges() { return std::set<Graph::Node*>(neigbours.begin(), neigbours.end());	}

	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);
	virtual std::string getClassName() { return "Room"; }
	static void shuffleTypes();
};