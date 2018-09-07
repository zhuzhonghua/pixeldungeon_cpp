#pragma once

#include <string>
#include "bundle.h"

class Statistics{
public:
	static int goldCollected;
	static int deepestFloor;
	static int enemiesSlain;
	static int foodEaten;
	static int potionsCooked;
	static int piranhasKilled;
	static int nightHunt;
	static int ankhsUsed;

	static float duration;

	static bool qualifiedForNoKilling;
	static bool completedWithNoKilling;

	static bool amuletObtained;

	static void reset();
	static void storeInBundle(Bundle* bundle);
	static void restoreFromBundle(Bundle* bundle);
private:
	static const std::string GOLD;
	static const std::string DEEPEST;
	static const std::string SLAIN;
	static const std::string FOOD;
	static const std::string ALCHEMY;
	static const std::string PIRANHAS;
	static const std::string NIGHT;
	static const std::string ANKHS;
	static const std::string DURATION;
	static const std::string AMULET;
};