#pragma once

#include "pixelscene.h"
#include "emitter.h"
#include "ripple.h"
#include "cellselector.h"
#include "wndbag.h"

class FloatingText;
class SkinnedBlock;
class DungeonTilemap;
class HeroSprite;
class GameLog;
class BusyIndicator;
class Item;
class FogOfWar;
class Toolbar;
class Toast;
class Blob;
class Mob;
class Plant;
class Banner;

class GameScene :public PixelScene{
private:
	static const std::string TXT_WELCOME;
	static const std::string TXT_WELCOME_BACK;
	static const std::string TXT_NIGHT_MODE;

	static const std::string TXT_CHASM;
	static const std::string TXT_WATER;
	static const std::string TXT_GRASS;
	static const std::string TXT_SECRETS;

	static GameScene* scene;

	SkinnedBlock* water;
	DungeonTilemap* tiles;
	FogOfWar* fog;
	HeroSprite* hero;

	GameLog* log;

	BusyIndicator* busy;

	static CellSelector* cellSelector;
	static CellSelector::Listener* defaultCellListener;

	Group* terrain;
	Group* ripples;
	Group* plants;
	Group* heaps;
	Group* mobs;
	Group* emitters;
	Group* effects;
	Group* gases;
	Group* spells;
	Group* statuses;
	Group* emoicons;

	Toolbar* toolbar;
	Toast* prompt;

	void addBlobSprite(Blob* gas);
	void addMobSprite(Mob* mob);
	void addPlantSprite(Plant* plant);
	void addHeapSprite(Heap* heap);

	void showBanner(Banner* banner);
	void Prompt(const std::string& text);

	static bool cancelCellSelector();
public:
	GameScene();
	void brightness(bool value);

	virtual void init();
	void destroy();

	static Emitter* emitter();
	static FloatingText* status();
	static Ripple* ripple(int pos);
	static void effect(Visual* eff);
	static void afterObserve();
	static void show(Window* wnd);

	static void ready();
	static bool cancel();

	static void addMob(Mob* mob, float delay);
	static void addMob(Mob* mob);

	static void addBlob(Blob* gas);

	virtual void update();

	static void handleCell(int cell);
	static void selectCell(CellSelector::Listener* listener);

	static void bossSlain();

	static void updateMap(int cell);
	static void discoverTile(int pos, int oldValue);
	static void pickUp(Item* item);

	static WndBag* selectItem(WndBag::Listener* listener, WndBag::Mode mode, const std::string& title);
};