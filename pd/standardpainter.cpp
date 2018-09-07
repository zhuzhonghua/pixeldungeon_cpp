#include "standardpainter.h"
#include "terrain.h"
#include "room.h"
#include "dungeon.h"
#include "util.h"

void StandardPainter::paint(Level* level, Room* room)
{
	fill(level, room->getBounds(), Terrain::WALL);
	for (std::map<Room*, Room::Door*>::iterator itr = room->connected.begin();
		itr != room->connected.end(); itr++)
	{
		itr->second->set(Room::Door::Type::REGULAR);
	}

	if (!Dungeon::bossLevel() && Random::Int(5) == 0) 
	{
		//switch (Random::Int(6)) 
		//{
		//case 0:
		//	if (level.feeling != Level.Feeling.GRASS) {
		//		if (Math.min(room.width(), room.height()) >= 4 && Math.max(room.width(), room.height()) >= 6) {
		//			paintGraveyard(level, room);
		//			return;
		//		}
		//		break;
		//	}
		//	else {
		//		// Burned room
		//	}
		//case 1:
		//	if (Dungeon.depth > 1) {
		//		paintBurned(level, room);
		//		return;
		//	}
		//	break;
		//case 2:
		//	if (Math.max(room.width(), room.height()) >= 4) {
		//		paintStriped(level, room);
		//		return;
		//	}
		//	break;
		//case 3:
		//	if (room.width() >= 6 && room.height() >= 6) {
		//		paintStudy(level, room);
		//		return;
		//	}
		//	break;
		//case 4:
		//	if (level.feeling != Level.Feeling.WATER) {
		//		if (room.connected.size() == 2 && room.width() >= 4 && room.height() >= 4) {
		//			paintBridge(level, room);
		//			return;
		//		}
		//		break;
		//	}
		//	else {
		//		// Fissure
		//	}
		//case 5:
		//	if (!Dungeon.bossLevel() && !Dungeon.bossLevel(Dungeon.depth + 1) &&
		//		Math.min(room.width(), room.height()) >= 5) {
		//		paintFissure(level, room);
		//		return;
		//	}
		//	break;
		//}
	}

	fill(level, room->getBounds(), 1, Terrain::EMPTY);
}
