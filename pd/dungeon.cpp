#include "dungeon.h"
#include "util.h"
#include "hero.h"
#include "level.h"
#include "pixeldungeon.h"
#include "statistics.h"
#include "sewerlevel.h"
#include "startscene.h"
#include "pathfinder.h"
#include "gamescene.h"
#include "belongings.h"
#include <sstream>

int Dungeon::potionOfStrength;
int Dungeon::scrollsOfUpgrade;
int Dungeon::scrollsOfEnchantment;
bool Dungeon::dewVial;		// true if the dew vial can be spawned

bool Dungeon::nightMode;

int Dungeon::challenges;

Hero* Dungeon::hero = NULL;
Level* Dungeon::level;

int Dungeon::depth = 0;
int Dungeon::gold = 0;

std::string Dungeon::resultDescription;

std::vector<bool> Dungeon::visible(Level::LENGTH);

std::set<int> Dungeon::chapters;

DroppedItemsType Dungeon::droppedItems;

Bundle* Dungeon::gameBundle(const std::string& fileName)
{
	std::stringstream ss;
	if (IOManager::readFileToBuffer(fileName, ss, true))
	{
		return Bundle::read(ss);
	}
	else
	{
		return NULL;
	}
}

std::string Dungeon::gameFile(const HeroClass& cl)
{
	switch (cl.type()) {
	case HeroClass::E_WARRIOR:
		return WR_GAME_FILE;
	case HeroClass::E_MAGE:
		return MG_GAME_FILE;
	case HeroClass::E_HUNTRESS:
		return RN_GAME_FILE;
	default:
		return RG_GAME_FILE;
	}
}

void Dungeon::preview(GamesInProgress::Info& info, Bundle* bundle)
{
	info.depth = bundle->getInt(DEPTH);
	info.challenges = (bundle->getInt(CHALLENGES) != 0);
	if (info.depth == -1) 
	{
		info.depth = bundle->getInt("maxDepth");	// FIXME
	}
	Bundle* tmp = bundle->getBundle(HERO);
	Hero::preview(info, tmp);
	delete tmp;
}

const std::string Dungeon::RG_GAME_FILE = "game.dat";
const std::string Dungeon::RG_DEPTH_FILE = "depth%d.dat";

const std::string Dungeon::WR_GAME_FILE = "warrior.dat";
const std::string Dungeon::WR_DEPTH_FILE = "warrior%d.dat";

const std::string Dungeon::MG_GAME_FILE = "mage.dat";
const std::string Dungeon::MG_DEPTH_FILE = "mage%d.dat";

const std::string Dungeon::RN_GAME_FILE = "ranger.dat";
const std::string Dungeon::RN_DEPTH_FILE = "ranger%d.dat";

const std::string Dungeon::VERSION = "version";
const std::string Dungeon::CHALLENGES = "challenges";
const std::string Dungeon::HERO = "hero";
const std::string Dungeon::GOLD = "gold";
const std::string Dungeon::DEPTH = "depth";
const std::string Dungeon::LEVEL = "level";
const std::string Dungeon::DROPPED = "dropped%d";
const std::string Dungeon::POS = "potionsOfStrength";
const std::string Dungeon::SOU = "scrollsOfEnhancement";
const std::string Dungeon::SOE = "scrollsOfEnchantment";
const std::string Dungeon::DV = "dewVial";
const std::string Dungeon::CHAPTERS = "chapters";
const std::string Dungeon::QUESTS = "quests";
const std::string Dungeon::BADGES = "badges";

std::vector<bool> Dungeon::passable(Level::LENGTH);

void Dungeon::init()
{
	challenges = PixelDungeon::challenges();

	Actor::clear();

	PathFinder::setMapSize(Level::WIDTH, Level::HEIGHT);
	//
	//Scroll.initLabels();
	//Potion.initColors();
	//Wand.initWoods();
	//Ring.initGems();
	//
	Statistics::reset();
	//Journal.reset();

	depth = 0;
	gold = 0;

	//droppedItems = new SparseArray<ArrayList<Item>>();

	potionOfStrength = 0;
	scrollsOfUpgrade = 0;
	scrollsOfEnchantment = 0;
	dewVial = true;

	chapters.clear();

	//Ghost.Quest.reset();
	//Wandmaker.Quest.reset();
	//Blacksmith.Quest.reset();
	//Imp.Quest.reset();
	//
	Room::shuffleTypes();
	//
	//QuickSlot.primaryValue = null;
	//QuickSlot.secondaryValue = null;

	hero = new Hero();
	hero->live();

	Badges::reset();

	StartScene::curClass.initHero(hero);
}

bool Dungeon::isChallenged(int mask)
{
	return (challenges & mask) != 0;
}

Level* Dungeon::newLevel()
{
	Dungeon::level = NULL;
	Actor::clear();

	depth++;
	if (depth > Statistics::deepestFloor) 
	{
		Statistics::deepestFloor = depth;

		if (Statistics::qualifiedForNoKilling) 
		{
			Statistics::completedWithNoKilling = true;
		}
		else 
		{
			Statistics::completedWithNoKilling = false;
		}
	}
	for (int i = 0; i < visible.size(); i++)
	{
		visible[i] = false;
	}

	Level* level;
	//switch (depth) {
	//case 1:
	//case 2:
	//case 3:
	//case 4:
		level = new SewerLevel();
	//	break;
	//case 5:
	//	level = new SewerBossLevel();
	//	break;
	//case 6:
	//case 7:
	//case 8:
	//case 9:
	//	level = new PrisonLevel();
	//	break;
	//case 10:
	//	level = new PrisonBossLevel();
	//	break;
	//case 11:
	//case 12:
	//case 13:
	//case 14:
	//	level = new CavesLevel();
	//	break;
	//case 15:
	//	level = new CavesBossLevel();
	//	break;
	//case 16:
	//case 17:
	//case 18:
	//case 19:
	//	level = new CityLevel();
	//	break;
	//case 20:
	//	level = new CityBossLevel();
	//	break;
	//case 21:
	//	level = new LastShopLevel();
	//	break;
	//case 22:
	//case 23:
	//case 24:
	//	level = new HallsLevel();
	//	break;
	//case 25:
	//	level = new HallsBossLevel();
	//	break;
	//case 26:
	//	level = new LastLevel();
	//	break;
	//default:
	//	level = new DeadEndLevel();
	//	Statistics.deepestFloor--;
	//}
	//
	level->create();
	//
	//Statistics::qualifiedForNoKilling = !bossLevel();

	return level;
}

void Dungeon::resetLevel()
{
	Actor::clear();

	for (int i = 0; i < visible.size(); i++)
	{
		visible[i] = false;
	}

	level->reset();
	switchLevel(level, level->entrance);
}

Level* Dungeon::loadLevel(HeroClass cl)
{
	Dungeon::level = NULL;
	Actor::clear();

	std::stringstream ss;
	IOManager::readFileToBuffer(GameMath::format(depthFile(cl).c_str(), depth), ss);
	Bundle* bundle = Bundle::read(ss);
	
	delete bundle;

	return NULL;// (Level)bundle.get("level");
}

void Dungeon::switchLevel(Level* level, int pos)
{
	//nightMode = new Date().getHours() < 7;

	Dungeon::level = level;
	Actor::init();

	//Actor respawner = level.respawner();
	//if (respawner != null) {
	//	Actor.add(level.respawner());
	//}
	//
	hero->pos = pos != -1 ? pos : level->exit;
	//
	//Light light = hero.buff(Light.class);
	//hero.viewDistance = light == null ? level.viewDistance : Math.max(Light.DISTANCE, level.viewDistance);
	//
	observe();
}

void Dungeon::observe()
{
	if (level == NULL) 
	{
		return;
	}

	level->updateFieldOfView(hero);
	Arrays<bool>::arraycopy(Level::fieldOfView, 0, visible, 0, visible.size());

	BArray::or(level->visited, visible, level->visited);

	GameScene::afterObserve();
}

void Dungeon::saveLevel()
{
	Bundle bundle;
	bundle.put(LEVEL, level);

	std::stringstream ss;
	Bundle::write(&bundle, ss);
	IOManager::writeFile(depthFile(hero->heroClass), ss.str());
}

std::string Dungeon::depthFile(HeroClass cl)
{
	switch (cl.type()) {
	case HeroClass::E_WARRIOR:
		return WR_DEPTH_FILE;
	case HeroClass::E_MAGE:
		return MG_DEPTH_FILE;
	case HeroClass::E_HUNTRESS:
		return RN_DEPTH_FILE;
	default:
		return RG_DEPTH_FILE;
	}
}

bool Dungeon::bossLevel()
{
	return bossLevel(depth);
}

int Dungeon::findPath(Char* ch, int from, int to, std::vector<bool>& pass, std::vector<bool>& visible)
{
	if (Level::adjacent(from, to)) 
	{
		return Actor::findChar(to) == NULL && (pass[to] || Level::avoid[to]) ? to : -1;
	}

	if (ch->flying || ch->buff("Amok") != NULL || ch->buff("Rage") != NULL) 
	{
		BArray::or(pass, Level::avoid, passable);
	}
	else 
	{
		Arrays<bool>::arraycopy(pass, 0, passable, 0, Level::LENGTH);
	}

	for (std::set<Actor*>::iterator itr = Actor::all.begin();
		itr != Actor::all.end(); itr++)
	{
		Actor* actor = *itr;
		if (dynamic_cast<Char*>(actor) != NULL)
		{
			int pos = ((Char*)actor)->pos;
			if (visible[pos]) 
			{
				passable[pos] = false;
			}
		}
	}

	return PathFinder::getStep(from, to, passable);
}

int Dungeon::flee(Char* ch, int cur, int from, std::vector<bool>& pass, std::vector<bool>& visible)
{
	if (ch->flying) 
	{
		BArray::or(pass, Level::avoid, passable);
	}
	else 
	{
		Arrays<bool>::arraycopy(pass, 0, passable, 0, Level::LENGTH);
	}

	for (std::set<Actor*>::iterator itr = Actor::all.begin();
		itr != Actor::all.end(); itr++)
	{
		Actor* actor = *itr;
		if (dynamic_cast<Char*>(actor) != NULL)
		{
			int pos = ((Char*)actor)->pos;
			if (visible[pos]) 
			{
				passable[pos] = false;
			}
		}
	}
	passable[cur] = true;

	return PathFinder::getStepBack(cur, from, passable);
}

void Dungeon::fail(const std::string& desc)
{
	resultDescription = desc;
	//if (hero.belongings.getItem(Ankh.class) == null) {
	//	Rankings.INSTANCE.submit(false);
	//}
}

void Dungeon::loadGame(HeroClass cl)
{
	loadGame(gameFile(cl), true);
}

void Dungeon::loadGame(const std::string& fileName)
{
	loadGame(fileName, false);
}

void Dungeon::loadGame(const std::string& fileName, bool fullLoad)
{
	Bundle* bundle = gameBundle(fileName);

	Dungeon::challenges = bundle->getInt(CHALLENGES);

	Dungeon::level = NULL;
	Dungeon::depth = -1;

	if (fullLoad) 
	{
		//PathFinder.setMapSize(Level.WIDTH, Level.HEIGHT);
	}

	//Scroll.restore(bundle);
	//Potion.restore(bundle);
	//Wand.restore(bundle);
	//Ring.restore(bundle);

	potionOfStrength = bundle->getInt(POS);
	scrollsOfUpgrade = bundle->getInt(SOU);
	scrollsOfEnchantment = bundle->getInt(SOE);
	dewVial = bundle->getBoolean(DV);

	if (fullLoad) 
	{
		chapters.clear();
		std::list<int> ids;
		bundle->getIntArray(CHAPTERS, ids);
		for (std::list<int>::iterator itr = ids.begin();
			itr != ids.end(); itr++)
		{
			chapters.insert(*itr);
		}

		Bundle* quests = bundle->getBundle(QUESTS);
		if (!quests->isNull()) 
		{
			//Ghost.Quest.restoreFromBundle(quests);
			//Wandmaker.Quest.restoreFromBundle(quests);
			//Blacksmith.Quest.restoreFromBundle(quests);
			//Imp.Quest.restoreFromBundle(quests);
		}
		else 
		{
			//Ghost.Quest.reset();
			//Wandmaker.Quest.reset();
			//Blacksmith.Quest.reset();
			//Imp.Quest.reset();
		}

		//Room.restoreRoomsFromBundle(bundle);
		delete quests;
	}

	Bundle* badges = bundle->getBundle(BADGES);
	if (!badges->isNull()) 
	{
		Badges::loadLocal(badges);
	}
	else 
	{
		Badges::reset();
	}
	delete badges;


	//QuickSlot.restore(bundle);

	std::string version = bundle->getString(VERSION);

	hero = NULL;
	//hero = (Hero)bundle.get(HERO);

	//QuickSlot.compress();

	gold = bundle->getInt(GOLD);
	depth = bundle->getInt(DEPTH);

	Statistics::restoreFromBundle(bundle);
	//Journal.restoreFromBundle(bundle);

	//droppedItems = new SparseArray<ArrayList<Item>>();
	//for (int i = 2; i <= Statistics.deepestFloor + 1; i++) {
	//	ArrayList<Item> dropped = new ArrayList<Item>();
	//	for (Bundlable b : bundle.getCollection(String.format(DROPPED, i))) {
	//		dropped.add((Item)b);
	//	}
	//	if (!dropped.isEmpty()) {
	//		droppedItems.put(i, dropped);
	//	}
	//}

	delete bundle;
}

void Dungeon::win(const String& desc)
{
	hero->belongings->identify();

	if (challenges != 0) {
		Badges::validateChampion();
	}

	resultDescription = desc;
	//Rankings.INSTANCE.submit(true);
}

void Dungeon::deleteGame(HeroClass cl, boolean deleteLevels)
{
	//Game.instance.deleteFile(gameFile(cl));
	//
	//if (deleteLevels) {
	//	int depth = 1;
	//	while (Game.instance.deleteFile(Utils.format(depthFile(cl), depth))) {
	//		depth++;
	//	}
	//}
	//
	//GamesInProgress.delete(cl);
}

void Dungeon::saveAll()
{
	if (hero->isAlive()) {

		Actor::fixTime();
		//saveGame(gameFile(hero.heroClass));
		saveLevel();

		//GamesInProgress::set(hero->heroClass, depth, hero->lvl, challenges != 0);

	}
	//else if (WndResurrect.instance != null) {
	//
	//	WndResurrect.instance.hide();
	//	Hero.reallyDie(WndResurrect.causeOfDeath);
	//
	//}
}
