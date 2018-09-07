#pragma once

#include "painter.h"

class Level;
class Room;
class StandardPainter :public Painter{
public:
	static void paint(Level* level, Room* room);
};