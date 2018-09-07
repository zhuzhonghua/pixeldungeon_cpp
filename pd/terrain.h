#pragma once

class Terrain{
public:
	static const int CHASM = 0;
	static const int EMPTY = 1;
	static const int GRASS = 2;
	static const int EMPTY_WELL = 3;
	static const int WALL = 4;
	static const int DOOR = 5;
	static const int OPEN_DOOR = 6;
	static const int ENTRANCE = 7;
	static const int EXIT = 8;
	static const int EMBERS = 9;
	static const int LOCKED_DOOR = 10;
	static const int PEDESTAL = 11;
	static const int WALL_DECO = 12;
	static const int BARRICADE = 13;
	static const int EMPTY_SP = 14;
	static const int HIGH_GRASS = 15;
	static const int EMPTY_DECO = 24;
	static const int LOCKED_EXIT = 25;
	static const int UNLOCKED_EXIT = 26;
	static const int SIGN = 29;
	static const int WELL = 34;
	static const int STATUE = 35;
	static const int STATUE_SP = 36;
	static const int BOOKSHELF = 41;
	static const int ALCHEMY = 42;
	static const int CHASM_FLOOR = 43;
	static const int CHASM_FLOOR_SP = 44;
	static const int CHASM_WALL = 45;
	static const int CHASM_WATER = 46;

	static const int SECRET_DOOR = 16;
	static const int TOXIC_TRAP = 17;
	static const int SECRET_TOXIC_TRAP = 18;
	static const int FIRE_TRAP = 19;
	static const int SECRET_FIRE_TRAP = 20;
	static const int PARALYTIC_TRAP = 21;
	static const int SECRET_PARALYTIC_TRAP = 22;
	static const int INACTIVE_TRAP = 23;
	static const int POISON_TRAP = 27;
	static const int SECRET_POISON_TRAP = 28;
	static const int ALARM_TRAP = 30;
	static const int SECRET_ALARM_TRAP = 31;
	static const int LIGHTNING_TRAP = 32;
	static const int SECRET_LIGHTNING_TRAP = 33;
	static const int GRIPPING_TRAP = 37;
	static const int SECRET_GRIPPING_TRAP = 38;
	static const int SUMMONING_TRAP = 39;
	static const int SECRET_SUMMONING_TRAP = 40;

	static const int WATER_TILES = 48;
	static const int WATER = 63;

	static const int PASSABLE = 0x01;
	static const int LOS_BLOCKING = 0x02;
	static const int FLAMABLE = 0x04;
	static const int SECRET = 0x08;
	static const int SOLID = 0x10;
	static const int AVOID = 0x20;
	static const int LIQUID = 0x40;
	static const int PIT = 0x80;

	static const int UNSTITCHABLE = 0x100;

	static int flags[256];

	static void init()
	{
		flags[CHASM] = AVOID | PIT | UNSTITCHABLE;
		flags[EMPTY] = PASSABLE;
		flags[GRASS] = PASSABLE | FLAMABLE;
		flags[EMPTY_WELL] = PASSABLE;
		flags[WATER] = PASSABLE | LIQUID | UNSTITCHABLE;
		flags[WALL] = LOS_BLOCKING | SOLID | UNSTITCHABLE;
		flags[DOOR] = PASSABLE | LOS_BLOCKING | FLAMABLE | SOLID | UNSTITCHABLE;
		flags[OPEN_DOOR] = PASSABLE | FLAMABLE | UNSTITCHABLE;
		flags[ENTRANCE] = PASSABLE/* | SOLID*/;
		flags[EXIT] = PASSABLE;
		flags[EMBERS] = PASSABLE;
		flags[LOCKED_DOOR] = LOS_BLOCKING | SOLID | UNSTITCHABLE;
		flags[PEDESTAL] = PASSABLE | UNSTITCHABLE;
		flags[WALL_DECO] = flags[WALL];
		flags[BARRICADE] = FLAMABLE | SOLID | LOS_BLOCKING;
		flags[EMPTY_SP] = flags[EMPTY] | UNSTITCHABLE;
		flags[HIGH_GRASS] = PASSABLE | LOS_BLOCKING | FLAMABLE;
		flags[EMPTY_DECO] = flags[EMPTY];
		flags[LOCKED_EXIT] = SOLID;
		flags[UNLOCKED_EXIT] = PASSABLE;
		flags[SIGN] = PASSABLE | FLAMABLE;
		flags[WELL] = AVOID;
		flags[STATUE] = SOLID;
		flags[STATUE_SP] = flags[STATUE] | UNSTITCHABLE;
		flags[BOOKSHELF] = flags[BARRICADE] | UNSTITCHABLE;
		flags[ALCHEMY] = PASSABLE;

		flags[CHASM_WALL] = flags[CHASM];
		flags[CHASM_FLOOR] = flags[CHASM];
		flags[CHASM_FLOOR_SP] = flags[CHASM];
		flags[CHASM_WATER] = flags[CHASM];

		flags[SECRET_DOOR] = flags[WALL] | SECRET | UNSTITCHABLE;
		flags[TOXIC_TRAP] = AVOID;
		flags[SECRET_TOXIC_TRAP] = flags[EMPTY] | SECRET;
		flags[FIRE_TRAP] = AVOID;
		flags[SECRET_FIRE_TRAP] = flags[EMPTY] | SECRET;
		flags[PARALYTIC_TRAP] = AVOID;
		flags[SECRET_PARALYTIC_TRAP] = flags[EMPTY] | SECRET;
		flags[POISON_TRAP] = AVOID;
		flags[SECRET_POISON_TRAP] = flags[EMPTY] | SECRET;
		flags[ALARM_TRAP] = AVOID;
		flags[SECRET_ALARM_TRAP] = flags[EMPTY] | SECRET;
		flags[LIGHTNING_TRAP] = AVOID;
		flags[SECRET_LIGHTNING_TRAP] = flags[EMPTY] | SECRET;
		flags[GRIPPING_TRAP] = AVOID;
		flags[SECRET_GRIPPING_TRAP] = flags[EMPTY] | SECRET;
		flags[SUMMONING_TRAP] = AVOID;
		flags[SECRET_SUMMONING_TRAP] = flags[EMPTY] | SECRET;
		flags[INACTIVE_TRAP] = flags[EMPTY];

		for (int i = WATER_TILES; i < WATER_TILES + 16; i++) 
		{
			flags[i] = flags[WATER];
		}
	}

	static int discover(int terr)
	{
		switch (terr) {
		case SECRET_DOOR:
			return DOOR;
		case SECRET_FIRE_TRAP:
			return FIRE_TRAP;
		case SECRET_PARALYTIC_TRAP:
			return PARALYTIC_TRAP;
		case SECRET_TOXIC_TRAP:
			return TOXIC_TRAP;
		case SECRET_POISON_TRAP:
			return POISON_TRAP;
		case SECRET_ALARM_TRAP:
			return ALARM_TRAP;
		case SECRET_LIGHTNING_TRAP:
			return LIGHTNING_TRAP;
		case SECRET_GRIPPING_TRAP:
			return GRIPPING_TRAP;
		case SECRET_SUMMONING_TRAP:
			return SUMMONING_TRAP;
		default:
			return terr;
		}
	}

	Terrain(){ init(); }

	static Terrain inst;
};