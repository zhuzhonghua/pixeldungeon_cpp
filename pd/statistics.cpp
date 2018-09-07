#include "statistics.h"

int Statistics::goldCollected;
int Statistics::deepestFloor;
int Statistics::enemiesSlain;
int Statistics::foodEaten;
int Statistics::potionsCooked;
int Statistics::piranhasKilled;
int Statistics::nightHunt;
int Statistics::ankhsUsed;

float Statistics::duration;

bool Statistics::qualifiedForNoKilling = false;
bool Statistics::completedWithNoKilling = false;

bool Statistics::amuletObtained = false;

const std::string Statistics::GOLD = "score";
const std::string Statistics::DEEPEST = "maxDepth";
const std::string Statistics::SLAIN = "enemiesSlain";
const std::string Statistics::FOOD = "foodEaten";
const std::string Statistics::ALCHEMY = "potionsCooked";
const std::string Statistics::PIRANHAS = "priranhas";
const std::string Statistics::NIGHT = "nightHunt";
const std::string Statistics::ANKHS = "ankhsUsed";
const std::string Statistics::DURATION = "duration";
const std::string Statistics::AMULET = "amuletObtained";

void Statistics::reset()
{
	goldCollected = 0;
	deepestFloor = 0;
	enemiesSlain = 0;
	foodEaten = 0;
	potionsCooked = 0;
	piranhasKilled = 0;
	nightHunt = 0;
	ankhsUsed = 0;

	duration = 0;

	qualifiedForNoKilling = false;

	amuletObtained = false;
}

void Statistics::storeInBundle(Bundle* bundle)
{
	bundle->put(GOLD, goldCollected);
	bundle->put(DEEPEST, deepestFloor);
	bundle->put(SLAIN, enemiesSlain);
	bundle->put(FOOD, foodEaten);
	bundle->put(ALCHEMY, potionsCooked);
	bundle->put(PIRANHAS, piranhasKilled);
	bundle->put(NIGHT, nightHunt);
	bundle->put(ANKHS, ankhsUsed);
	bundle->put(DURATION, duration);
	bundle->put(AMULET, amuletObtained);
}

void Statistics::restoreFromBundle(Bundle* bundle)
{
	goldCollected = bundle->getInt(GOLD);
	deepestFloor = bundle->getInt(DEEPEST);
	enemiesSlain = bundle->getInt(SLAIN);
	foodEaten = bundle->getInt(FOOD);
	potionsCooked = bundle->getInt(ALCHEMY);
	piranhasKilled = bundle->getInt(PIRANHAS);
	nightHunt = bundle->getInt(NIGHT);
	ankhsUsed = bundle->getInt(ANKHS);
	duration = bundle->getFloat(DURATION);
	amuletObtained = bundle->getBoolean(AMULET);
}