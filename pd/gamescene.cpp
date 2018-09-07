#include "gamescene.h"
#include "floatingtext.h"
#include "pixeldungeon.h"
#include "dungeon.h"
#include "skinnedblock.h"
#include "dungeontilemap.h"
#include "fogofwar.h"
#include "herosprite.h"
#include "cellselector.h"
#include "toolbar.h"
#include "gamelog.h"
#include "busyindicator.h"
#include "interlevelscene.h"
#include "wndstory.h"
#include "statistics.h"
#include "glog.h"
#include "statuspane.h"
#include "regularlevel.h"
#include "blob.h"
#include "blobemitter.h"
#include "mob.h"
#include "healthindicator.h"
#include "attackindicator.h"
#include "toast.h"
#include "bpt.h"
#include "plant.h"
#include "plantsprite.h"
#include "heap.h"
#include "wand.h"
#include "flare.h"
#include "chasm.h"
#include "potion.h"
#include "plant.h"
#include "quickslot.h"
#include "bannersprites.h"
#include "banner.h"
#include "hero.h"

const std::string GameScene::TXT_WELCOME = BPT::getText("lang.welcom_to_level"); //"Welcome to the level %d of Pixel Dungeon!";
const std::string GameScene::TXT_WELCOME_BACK = BPT::getText("lang.welcom_back_to_level"); //"Welcome back to the level %d of Pixel Dungeon!";
const std::string GameScene::TXT_NIGHT_MODE = BPT::getText("lang.be_caution_night"); //"Be cautious, since the dungeon is even more dangerous at night!";
const std::string GameScene::TXT_CHASM = BPT::getText("lang.step_echo_dungeon"); //"Your steps echo across the dungeon.";
const std::string GameScene::TXT_WATER = BPT::getText("lang.hear_water_splash"); //"You hear the water splashing around you.";
const std::string GameScene::TXT_GRASS = BPT::getText("lang.smell_vegetation_air"); //"The smell of vegetation is thick in the air.";
const std::string GameScene::TXT_SECRETS = BPT::getText("lang.floor_hide_secrets"); //"The atmosphere hints that this floor hides many secrets.";

GameScene* GameScene::scene;
CellSelector* GameScene::cellSelector;

namespace{
	class CellSelectorListenerNew :public CellSelector::Listener{
	public:
		virtual void onSelect(int cell)
		{
			if (Dungeon::hero->handle(cell))
			{
				Dungeon::hero->next();
			}
		}

		virtual std::string prompt() { return ""; }
	};
}

CellSelector::Listener* GameScene::defaultCellListener = new CellSelectorListenerNew();

void GameScene::addBlobSprite(Blob* gas)
{
	if (gas->emitter == NULL) 
	{
		gases->add(new BlobEmitter(gas));
	}
}

void GameScene::addMobSprite(Mob* mob)
{
	CharSprite* sprite = mob->Sprite();
	sprite->visible = Dungeon::visible[mob->pos];
	mobs->add(sprite);
	sprite->link(mob);
}

void GameScene::addPlantSprite(Plant* plant)
{
	RECYCLE(plant->sprite, plants, PlantSprite);
	//plant->sprite = (PlantSprite*)plants->recycle("PlantSprite");
	//f (plant->sprite == NULL)
	//
	//	plant->sprite = new PlantSprite();
	//	plants->add(plant->sprite);
	//
	plant->sprite->reset(plant);
}

void GameScene::addHeapSprite(Heap* heap)
{
	RECYCLE(heap->sprite, heaps, ItemSprite);
	//ItemSprite* sprite = heap->sprite = (ItemSprite*)heaps->recycle("ItemSprite");
	//if (sprite == NULL)
	//{
	//	sprite = heap->sprite = new ItemSprite();
	//	heaps->add(sprite);
	//}
	ItemSprite* sprite = heap->sprite;
	sprite->revive();
	sprite->link(heap);
}

namespace{
	class ToastNew :public Toast{
	public:
		GameScene* sce;
		ToastNew(GameScene* s, const std::string& text)
			:Toast(text), sce(s)
		{

		}
	protected:
		virtual void onClose()
		{
			sce->cancel();
		}
	};
}
void GameScene::Prompt(const std::string& text)
{
	if (prompt != NULL) 
	{
		prompt->killAndErase();
		delete prompt;
		prompt = NULL;
	}

	if (text.size() > 0) 
	{
		prompt = new ToastNew(this, text);
		prompt->cameraf = uiCamera;
		prompt->setPos((uiCamera->width - prompt->width()) / 2, uiCamera->height - 60);
		add(prompt);
	}
}

bool GameScene::cancelCellSelector()
{
	if (cellSelector->listener != NULL && cellSelector->listener != defaultCellListener) 
	{
		cellSelector->cancel();
		return true;
	}
	else 
	{
		return false;
	}
}

void GameScene::brightness(bool value)
{
	water->rm = water->gm = water->bm =
		tiles->rm = tiles->gm = tiles->bm =
		value ? 1.5f : 1.0f;
	if (value) 
	{
		fog->am = +2.0f;
		fog->aa = -1.0f;
	}
	else 
	{
		fog->am = +1.0f;
		fog->aa = 0.0f;
	}
}

GameScene::GameScene()
{
	prompt = NULL;
}

void GameScene::init()
{
	//Music.INSTANCE.play(Assets.TUNE, true);
	//Music.INSTANCE.volume(1f);

	PixelDungeon::lastClass(Dungeon::hero->heroClass.ordinal());

	PixelScene::init();
	Camera::mainCamera->zoom(defaultZoom + PixelDungeon::zoom());

	scene = this;

	terrain = new Group();
	add(terrain);

	water = new SkinnedBlock(
		Level::WIDTH * DungeonTilemap::SIZE,
		Level::HEIGHT * DungeonTilemap::SIZE,
		Dungeon::level->waterTex());
	terrain->add(water);
	
	ripples = new Group();
	terrain->add(ripples);

	tiles = new DungeonTilemap();
	terrain->add(tiles);

	Dungeon::level->addVisuals(this);

	plants = new Group();
	add(plants);

	int size = Dungeon::level->plants.size();
	for (HashMap<int, Plant*>::iterator itr = Dungeon::level->plants.begin();
		itr != Dungeon::level->plants.end(); itr++)
	{
		addPlantSprite(itr->second);
	}

	heaps = new Group();
	add(heaps);

	for (HashMap<int, Heap*>::iterator itr = Dungeon::level->heaps.begin();
		itr != Dungeon::level->heaps.end(); itr++)
	{
		addHeapSprite(itr->second);
	}

	emitters = new Group();
	effects = new Group();
	emoicons = new Group();
	
	mobs = new Group();
	add(mobs);

	for (std::set<Mob*>::iterator itr = Dungeon::level->mobs.begin();
		itr != Dungeon::level->mobs.end(); itr++)
	{
		Mob* mob = *itr;
		addMobSprite(mob);
		if (Statistics::amuletObtained) 
		{
			mob->beckon(Dungeon::hero->pos);
		}
	}
	add(emitters);
	add(effects);
	
	gases = new Group();
	add(gases);

	for (std::map<std::string, Blob*>::iterator itr = Dungeon::level->blobs.begin();
		itr != Dungeon::level->blobs.end(); itr++)
	{
		Blob* blob = itr->second;
		blob->emitter = NULL;
		addBlobSprite(blob);
	}

	fog = new FogOfWar(Level::WIDTH, Level::HEIGHT);
	fog->updateVisibility(Dungeon::visible, Dungeon::level->visited, Dungeon::level->mapped);
	add(fog);
	
	brightness(PixelDungeon::brightness());
	
	spells = new Group();
	add(spells);
	
	statuses = new Group();
	add(statuses);
	
	add(emoicons);
	//

	hero = new HeroSprite();
	hero->place(Dungeon::hero->pos);
	hero->updateArmor();
	mobs->add(hero);

	add(new HealthIndicator());

	add(cellSelector = new CellSelector(tiles));

	StatusPane* sb = new StatusPane();
	sb->cameraf = uiCamera;
	sb->setSize(uiCamera->width, 0);
	add(sb);

	toolbar = new Toolbar();
	toolbar->cameraf = uiCamera;
	toolbar->setRect(0, uiCamera->height - toolbar->height(), uiCamera->width, toolbar->height());
	add(toolbar);

	AttackIndicator* attack = new AttackIndicator();
	attack->cameraf = uiCamera;
	attack->setPos(
		uiCamera->width - attack->width(),
		toolbar->top() - attack->height());
	add(attack);

	log = new GameLog();
	log->cameraf = uiCamera;
	log->setRect(0, toolbar->top(), attack->left(), 0);
	add(log);

	busy = new BusyIndicator();
	busy->cameraf = uiCamera;
	busy->x = 1;
	busy->y = sb->bottom() + 1;
	add(busy);

	Flare* f = NULL;
	switch (InterlevelScene::mode) 
	{
	case InterlevelScene::RESURRECT:
		WandOfBlink::appear(Dungeon::hero, Dungeon::level->entrance);
		f = new Flare(8, 32);
		f->color(0xFFFF66, true);
		f->show(hero, 2.0f);
		break;
	case InterlevelScene::RETURN:
		WandOfBlink::appear(Dungeon::hero, Dungeon::hero->pos);
		break;
	case InterlevelScene::FALL:
		Chasm::heroLand();
		break;
	case InterlevelScene::DESCEND:
		switch (Dungeon::depth) {
		case 1:
			WndStory::showChapter(WndStory::ID_SEWERS);
			break;
		case 6:
			WndStory::showChapter(WndStory::ID_PRISON);
			break;
		case 11:
			WndStory::showChapter(WndStory::ID_CAVES);
			break;
		case 16:
			WndStory::showChapter(WndStory::ID_METROPOLIS);
			break;
		case 22:
			WndStory::showChapter(WndStory::ID_HALLS);
			break;
		}
		if (Dungeon::hero->isAlive() && Dungeon::depth != 22) 
		{
			Badges::validateNoKilling();
		}
		break;
	default:
		break;
	}

	DroppedItemsType::iterator it = Dungeon::droppedItems.find(Dungeon::depth);
	if (it != Dungeon::droppedItems.end())
	{
		ArrayList<Item*>& dropped = it->second;
		for (ArrayList<Item*>::iterator itr = dropped.begin();
			itr != dropped.end(); itr++)
		{
			Item* item = *itr;
			int pos = Dungeon::level->randomRespawnCell();
			if (dynamic_cast<Potion*>(item)) {
				((Potion*)item)->shatter(pos);
			}
			else if (dynamic_cast<Plant::Seed*>(item)) {
				Dungeon::level->plant((Plant::Seed*)item, pos);
			}
			else {
				Dungeon::level->drop(item, pos);
			}
		}
		Dungeon::droppedItems.erase(it);
	}

	Camera::mainCamera->target = hero;

	if (InterlevelScene::mode != InterlevelScene::Mode::NONE) 
	{
		if (Dungeon::depth < Statistics::deepestFloor) 
		{
			GLog::h(TXT_WELCOME_BACK, Dungeon::depth);
		}
		else 
		{
			GLog::h(TXT_WELCOME, Dungeon::depth);
			//Sample.INSTANCE.play(Assets.SND_DESCEND);
		}
		switch (Dungeon::level->feeling) 
		{
		case Level::CHASM:
			GLog::w(TXT_CHASM);
			break;
		case Level::WATER:
			GLog::w(TXT_WATER);
			break;
		case Level::GRASS:
			GLog::w(TXT_GRASS);
			break;
		default:
			break;
		}
		if (dynamic_cast<RegularLevel*>(Dungeon::level) != NULL &&
			((RegularLevel*)Dungeon::level)->secretDoors > Random::IntRange(3, 4)) 
		{
			GLog::w(TXT_SECRETS);
		}
		if (Dungeon::nightMode && !Dungeon::bossLevel()) 
		{
			GLog::w(TXT_NIGHT_MODE);
		}

		InterlevelScene::mode = InterlevelScene::Mode::NONE;

		fadeIn();
	}
}

void GameScene::destroy()
{
	scene = NULL;
	Badges::saveGlobal();

	PixelScene::destroy();
}

Emitter* GameScene::emitter()
{
	if (scene != NULL) 
	{
		Emitter* emitter = (Emitter*)scene->emitters->recycle("Emitter");
		if (emitter == NULL)
		{
			emitter = new Emitter();
			scene->emitters->add(emitter);
		}
		emitter->revive();
		return emitter;
	}
	else 
	{
		return NULL;
	}
}

FloatingText* GameScene::status()
{
	if (scene == NULL) return NULL;

	RECYCLE2(txt, scene->statuses, FloatingText);
	//FloatingText* txt = (FloatingText*)scene->statuses->recycle("FloatingText");
	//if (txt == NULL)
	//{
	//	txt = new FloatingText();
	//}
	
	return txt;
}

Ripple* GameScene::ripple(int pos)
{
	RECYCLE2(ripple, scene->ripples, Ripple);
	//Ripple* ripple = (Ripple*)scene->ripples->recycle("Ripple");
	//if (ripple == NULL)
	//{
	//	ripple = new Ripple();
	//	scene->ripples->add(ripple);
	//}
	ripple->reset(pos);
	return ripple;
}

void GameScene::effect(Visual* eff)
{
	scene->effects->add(eff);
}

void GameScene::afterObserve()
{
	if (scene != NULL) 
	{
		scene->fog->updateVisibility(Dungeon::visible, Dungeon::level->visited, Dungeon::level->mapped);

		for (std::set<Mob*>::iterator itr = Dungeon::level->mobs.begin();
			itr != Dungeon::level->mobs.end(); itr++)
		{
			Mob* mob = *itr;
			mob->sprite->visible = Dungeon::visible[mob->pos];
		}
	}
}

void GameScene::show(Window* wnd)
{
	cancelCellSelector();
	scene->add(wnd);
}

void GameScene::ready()
{
	selectCell(defaultCellListener);
	QuickSlot::cancel();
}

bool GameScene::cancel()
{
	if (Dungeon::hero->curAction != NULL || Dungeon::hero->restoreHealth) 
	{
		Dungeon::hero->curAction = NULL;
		Dungeon::hero->restoreHealth = false;
		return true;	
	}
	else 
	{
		return cancelCellSelector();
	}
	return true;
}

void GameScene::addMob(Mob* mob, float delay)
{
	Dungeon::level->mobs.insert(mob);
	Actor::addDelayed(mob, delay);
	Actor::occupyCell(mob);
	scene->addMobSprite(mob);
}

void GameScene::addMob(Mob* mob)
{
	Dungeon::level->mobs.insert(mob);
	Actor::add(mob);
	Actor::occupyCell(mob);
	scene->addMobSprite(mob);
}

void GameScene::addBlob(Blob* gas)
{
	Actor::add(gas);
	if (scene != NULL) {
		scene->addBlobSprite(gas);
	}
}

void GameScene::update()
{
	if (Dungeon::hero == NULL) 
	{
		return;
	}

	PixelScene::update();

	water->offset(0, -5 * Game::elapsed);

	Actor::process();

	if (Dungeon::hero->ready && !Dungeon::hero->paralysed) 
	{
		log->newLine();
	}

	cellSelector->enabled = Dungeon::hero->ready;
}

void GameScene::handleCell(int cell)
{
	cellSelector->select(cell);
}

void GameScene::selectCell(CellSelector::Listener* listener)
{
	cellSelector->listener = listener;
	scene->Prompt(listener->prompt());
}

void GameScene::bossSlain()
{
	if (Dungeon::hero->isAlive()) 
	{
		Banner* bossSlain = new Banner(BannerSprites::get(BannerSprites::Type::BOSS_SLAIN));
		bossSlain->show(0xFFFFFF, 0.3f, 5.0f);
		scene->showBanner(bossSlain);

		//Sample.INSTANCE.play(Assets.SND_BOSS);
	}
}

void GameScene::updateMap(int cell)
{
	if (scene != NULL) 
	{
		GameMath::RECTUnion(&scene->tiles->updated, cell % Level::WIDTH, cell / Level::WIDTH);
	}
}

void GameScene::discoverTile(int pos, int oldValue)
{
	if (scene != NULL) {
		scene->tiles->discover(pos, oldValue);
	}
}

void GameScene::pickUp(Item* item)
{
	scene->toolbar->pickup(item);
}

WndBag* GameScene::selectItem(WndBag::Listener* listener, WndBag::Mode mode, const std::string& title)
{
	cancelCellSelector();

	WndBag* wnd = mode == WndBag::Mode::SEED ?
		WndBag::seedPouch(listener, mode, title) :
		WndBag::LastBag(listener, mode, title);
	scene->add(wnd);

	return wnd;
}

void GameScene::showBanner(Banner* banner)
{
	banner->cameraf = uiCamera;
	banner->x = align(uiCamera, (uiCamera->width - banner->width) / 2);
	banner->y = align(uiCamera, (uiCamera->height - banner->height) / 3);
	add(banner);
}
