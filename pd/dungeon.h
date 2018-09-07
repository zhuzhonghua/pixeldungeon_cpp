#pragma once

#include <map>
#include "bundle.h"
#include "heroclass.h"
#include "gamesinprogress.h"
//#include "hero.h"
#include "level.h"

class Char;

typedef std::map<int, ArrayList< Item* > > DroppedItemsType;

class Dungeon{
public:
	static Bundle* gameBundle(const std::string& fileName);
	static std::string gameFile(const HeroClass& cl);
	static void preview(GamesInProgress::Info& info, Bundle* bundle);

	static int potionOfStrength;
	static int scrollsOfUpgrade;
	static int scrollsOfEnchantment;
	static bool dewVial;		// true if the dew vial can be spawned

	static int challenges;

	static Hero* hero;
	static Level* level;

	static int depth;
	static int gold;

	static std::string resultDescription;

	static DroppedItemsType droppedItems;
	static bool nightMode;
	static std::vector<bool> visible;
	static std::set<int> chapters;

	static void init();
	static bool isChallenged(int mask);
	static Level* newLevel();

	static void loadGame(HeroClass cl);
	static void loadGame(const std::string& fileName);
	static void loadGame(const std::string& fileName, bool fullLoad);

	static void resetLevel();
	static void saveLevel();
	static Level* loadLevel(HeroClass cl);
	static void switchLevel(Level* level, int pos);
	static void observe();

	static bool shopOnLevel() { return depth == 6 || depth == 11 || depth == 16; }
	static bool bossLevel();
	static bool bossLevel(int depth){ return depth == 5 || depth == 10 || depth == 15 || depth == 20 || depth == 25; }
	static int findPath(Char* ch, int from, int to, std::vector<bool>& pass, std::vector<bool>& visible);
	static int flee(Char* ch, int cur, int from, std::vector<bool>& pass, std::vector<bool>& visible);
	static void fail(const std::string& desc);
	static void win(const String& desc);
	static void deleteGame(HeroClass cl, boolean deleteLevels);
	static void saveAll();
private:
	static const std::string RG_GAME_FILE ;
	static const std::string RG_DEPTH_FILE ;
	static const std::string WR_GAME_FILE ;
	static const std::string WR_DEPTH_FILE ;
	static const std::string MG_GAME_FILE ;
	static const std::string MG_DEPTH_FILE ;
	static const std::string RN_GAME_FILE ;
	static const std::string RN_DEPTH_FILE ;
	static const std::string VERSION ;
	static const std::string CHALLENGES ;
	static const std::string HERO ;
	static const std::string GOLD ;
	static const std::string DEPTH ;
	static const std::string LEVEL ;
	static const std::string DROPPED ;
	static const std::string POS ;
	static const std::string SOU ;
	static const std::string SOE ;
	static const std::string DV ;
	static const std::string CHAPTERS ;
	static const std::string QUESTS ;
	static const std::string BADGES ;

	static std::vector<bool> passable;
	static std::string depthFile(HeroClass cl);
};