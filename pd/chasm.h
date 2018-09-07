#pragma once
#include <string>

class Hero;
class Mob;

class Chasm{
private:
	static const std::string TXT_CHASM;
	static const std::string TXT_YES;
	static const std::string TXT_NO;
	static const std::string TXT_JUMP;

public:
	static bool jumpConfirmed;

	static void heroJump(Hero* hero);
	static void heroFall(int pos);
	static void heroLand();
	static void mobFall(Mob* mob);
};