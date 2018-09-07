#pragma once

#include "typedefine.h"

class Mob;
class Char;

class Bestiary{
public:
	static Mob* mob(int depth);
	static boolean isBoss(Char* mob);
private:
	static Mob* mobClass(int depth);
};