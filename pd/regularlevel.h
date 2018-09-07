#pragma once

#include "level.h"
#include "define.h"
#include "room.h"

#include <set>

class RegularLevel :public Level{
public:
	int secretDoors;

	RegularLevel();
	virtual ~RegularLevel();

	virtual int nMobs();
	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);

	Room* room(int pos);
protected:
	std::list<Room::Type> specials;
	std::set<Room*> rooms;
	int minRoomSize;
	int maxRoomSize;

	Room* roomEntrance;
	Room* roomExit;

	virtual bool build();
	bool initRooms();
	void split(Rect rect);
	void paint();
	void paintWater();
	void paintGrass();
	void placeTraps();

	void paintDoors(Room* r);
	void assignRoomType();
	Room* randomRoom(Room::Type type, int tries);
	int randomDropCell();
	
	bool joinRooms(Room* r, Room* n);
	int nTraps();
	std::vector<float> trapChances();

	virtual void createMobs();
	virtual void createItems();
	virtual std::vector<bool> water() = 0;
	virtual std::vector<bool> grass() = 0;
private:
	void placeDoors(Room* r);
};