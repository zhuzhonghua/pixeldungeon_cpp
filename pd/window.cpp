#include "window.h"
#include "simpleresource.h"
#include "pixelscene.h"
#include "game.h"
#include "dungeon.h"
#include "level.h"
#include "terrain.h"
#include "icontitle.h"
#include "dungeontilemap.h"
#include "blob.h"
#include "pixeldungeon.h"
#include "icon.h"
#include "heap.h"
#include "itemslot.h"
#include "wndbag.h"
#include "ring.h"
#include "glog.h"
#include "belongings.h"
#include "gold.h"
#include "redbutton.h"
#include "journal.h"
#include "scrollpane.h"
#include "hero.h"

namespace{
	class NewTouchArea :public TouchArea{
	public:
		Window* win;

		NewTouchArea(Window* w, float x, float y, float width, float height) :TouchArea(x, y, width, height)
		{
			win = w;
		}
	protected:
		virtual void onClick(TouchScreen::Touch* touch)
		{
			if (!win->chrome->overlapsScreenPoint(
				(int)touch->current.x,
				(int)touch->current.y)) 
			{

				win->onBackPressed();
			}
		}
	};	
}
Window::Window()
:Window(0, 0, Chrome::get(Chrome::Type::WINDOW))
{
}

Window::Window(int width, int height)
: Window(width, height, Chrome::get(Chrome::Type::WINDOW))
{

}

Window::Window(int width, int height, NinePatch* chrome)
: Group()
{
	blocker = new NewTouchArea(this, 0, 0, PixelScene::uiCamera->width, PixelScene::uiCamera->height);
	blocker->cameraf = PixelScene::uiCamera;
	add(blocker);

	this->chrome = chrome;

	this->width = width;
	this->height = height;

	shadow = new ShadowBox();
	shadow->am = 0.5f;
	shadow->cameraf = PixelScene::uiCamera->visible ? PixelScene::uiCamera : Camera::mainCamera;
	add(shadow);

	chrome->x = -chrome->marginLeft();
	chrome->y = -chrome->marginTop();
	chrome->size(
		width - chrome->x + chrome->marginRight(),
		height - chrome->y + chrome->marginBottom());
	add(chrome);

	cameraf = new Camera(0, 0,
		(int)chrome->width,
		(int)chrome->height,
		PixelScene::defaultZoom);
	cameraf->x = (int)(Game::width - cameraf->width * cameraf->zoomf) / 2;
	cameraf->y = (int)(Game::height - cameraf->height * cameraf->zoomf) / 2;
	GameMath::PointFSet(&cameraf->scroll, chrome->x, chrome->y);
	//cameraf->scroll.set(chrome->x, chrome->y);
	Camera::add(cameraf);

	shadow->boxRect(
		cameraf->x / cameraf->zoomf,
		cameraf->y / cameraf->zoomf,
		chrome->getWidth(), chrome->height);

	Keys::event.add(this);
}

void Window::resize(int w, int h)
{
	this->width = w;
	this->height = h;

	chrome->size(
		width + chrome->marginHor(),
		height + chrome->marginVer());

	cameraf->resize((int)chrome->width, (int)chrome->height);
	cameraf->x = (int)(Game::width - cameraf->screenWidth()) / 2;
	cameraf->y = (int)(Game::height - cameraf->screenHeight()) / 2;

	shadow->boxRect(cameraf->x / cameraf->zoomf, cameraf->y / cameraf->zoomf, chrome->getWidth(), chrome->height);
}

void Window::destroy()
{
	Group::destroy();

	Camera::remove(cameraf);
	Keys::event.remove(this);
}

void Window::onSignal(Keys::Key* key)
{
	/*if (key.pressed) {
		switch (key.code) {
		case Keys.BACK:
			onBackPressed();
			break;
		case Keys.MENU:
			onMenuPressed();
			break;
		}
	}*/

	Keys::event.cancel();
}

const float WndInfoCell::GAP = 2;

const String WndInfoCell::TXT_NOTHING = "There is nothing here.";

WndInfoCell::WndInfoCell(int cell)
{
	int tile = Dungeon::level->map[cell];
	if (Level::water[cell]) {
		tile = Terrain::WATER;
	}
	else if (Level::pit[cell]) {
		tile = Terrain::CHASM;
	}

	IconTitle* titlebar = new IconTitle();
	if (tile == Terrain::WATER) {
		Image* water = new Image(Dungeon::level->waterTex());
		water->frame(0, 0, DungeonTilemap::SIZE, DungeonTilemap::SIZE);
		titlebar->icon(water);
	}
	else {
		titlebar->icon(DungeonTilemap::tile(tile));
	}
	titlebar->label(Dungeon::level->tileName(tile));
	titlebar->setRect(0, 0, WIDTH, 0);
	add(titlebar);

	BitmapTextMultiline* info = PixelScene::createMultiline(6);
	add(info);

	std::stringstream desc;
	desc << Dungeon::level->tileDesc(tile);

	char newLine = '\n';
	for (std::map<std::string, Blob*>::iterator itr = Dungeon::level->blobs.begin();
		itr != Dungeon::level->blobs.end(); itr++)
	{
		Blob* blob = itr->second;
		if (blob->cur[cell] > 0 && blob->tileDesc().size() > 0) {
			if (desc.str().size() > 0) {
				desc << newLine;
			}
			desc << blob->tileDesc();
		}
	}

	info->text(desc.str().length() > 0 ? desc.str() : TXT_NOTHING);
	info->maxWidth = WIDTH;
	info->measure();
	info->x = titlebar->left();
	info->y = titlebar->bottom() + GAP;

	resize(WIDTH, (int)(info->y + info->getHeight()));
}

const String WndJournal::TXT_TITLE = "Journal";

WndJournal::WndJournal()
{
	resize(WIDTH, PixelDungeon::landscape() ? HEIGHT_L : HEIGHT_P);

	txtTitle = PixelScene::createText(TXT_TITLE, 9);
	txtTitle->hardlight(Window::TITLE_COLOR);
	txtTitle->measure();
	txtTitle->x = PixelScene::align(PixelScene::uiCamera, (WIDTH - txtTitle->getWidth()) / 2);
	add(txtTitle);

	Component* content = new Component();

	Journal::records.sort();
	float pos = 0;
	for (ArrayList<Journal::Record>::iterator itr = Journal::records.begin();
		itr != Journal::records.end(); itr++)
	{
		ListItem* item = new ListItem(itr->feature, itr->depth);
		item->setRect(0, pos, WIDTH, ITEM_HEIGHT);
		content->add(item);

		pos += item->height();
	}
	
	content->setSize(WIDTH, pos);

	list = new ScrollPane(content);
	add(list);

	list->setRect(0, txtTitle->getHeight(), WIDTH, height - txtTitle->getHeight());
}

WndJournal::ListItem::ListItem( Journal::Feature f, int d)
{
	init();

	feature->text(f.desc);
	feature->measure();
	
	std::stringstream ss;
	ss << d;

	depth->text(ss.str());
	depth->measure();
	
	if (d == Dungeon::depth) {
		feature->hardlight(TITLE_COLOR);
		depth->hardlight(TITLE_COLOR);
	}
}

void WndJournal::ListItem::createChildren()
{
	feature = PixelScene::createText(9);
	add(feature);

	depth = PixelScene::createText(9);
	add(depth);

	icon = Icons::get(Icons::DEPTH);
	add(icon);
}

void WndJournal::ListItem::layout()
{
	icon->x = _width - icon->width;

	depth->x = icon->x - 1 - depth->getWidth();
	depth->y = PixelScene::align(_y + (_height - depth->getHeight()) / 2);

	icon->y = depth->y - 1;

	feature->y = PixelScene::align(depth->y + depth->baseLine() - feature->baseLine());
}

const String WndInfoItem::TXT_CHEST = "Chest";

const String WndInfoItem::TXT_LOCKED_CHEST = "Locked chest";

const String WndInfoItem::TXT_CRYSTAL_CHEST = "Crystal chest";

const String WndInfoItem::TXT_TOMB = "Tomb";

const String WndInfoItem::TXT_SKELETON = "Skeletal remains";

const String WndInfoItem::TXT_WONT_KNOW = "You won't know what's inside until you open it!";

const String WndInfoItem::TXT_NEED_KEY = TXT_WONT_KNOW + " But to open it you need a golden key.";

const String WndInfoItem::TXT_INSIDE = "You can see %s inside, but to open the chest you need a golden key.";

const String WndInfoItem::TXT_OWNER =
std::string("This ancient tomb may contain something useful, ") +
std::string("but its owner will most certainly object to checking.");

const String WndInfoItem::TXT_REMAINS =
std::string("This is all that's left from one of your predecessors. ") +
std::string("Maybe it's worth checking for any valuables.");

const float WndInfoItem::GAP = 2;

void WndInfoItem::fillFields(int image, ItemSprite::Glowing* glowing, int titleColor, const String& title, const String& info)
{
	IconTitle* titlebar = new IconTitle();
	titlebar->icon(new ItemSprite(image, glowing));
	titlebar->label(title, titleColor);
	titlebar->setRect(0, 0, WIDTH, 0);
	add(titlebar);

	BitmapTextMultiline* txtInfo = PixelScene::createMultiline(info, 6);
	txtInfo->maxWidth = WIDTH;
	txtInfo->measure();
	txtInfo->x = titlebar->left();
	txtInfo->y = titlebar->bottom() + GAP;
	add(txtInfo);

	resize(WIDTH, (int)(txtInfo->y + txtInfo->getHeight()));
}

WndInfoItem::WndInfoItem(Heap* heap)
{
	if (heap->type == Heap::Type::HEAP || heap->type == Heap::Type::FOR_SALE) {

		Item* item = heap->peek();

		int color = TITLE_COLOR;
		if (item->levelKnown) {
			if (item->Level() < 0) {
				color = ItemSlot::DEGRADED;
			}
			else if (item->Level() > 0) {
				color = item->isBroken() ? ItemSlot::WARNING : ItemSlot::UPGRADED;
			}
		}
		fillFields(item->Image(), item->glowing(), color, item->toString(), item->info());

	}
	else {

		String title;
		String info;

		if (heap->type == Heap::Type::CHEST || heap->type == Heap::Type::MIMIC) {
			title = TXT_CHEST;
			info = TXT_WONT_KNOW;
		}
		else if (heap->type == Heap::Type::TOMB) {
			title = TXT_TOMB;
			info = TXT_OWNER;
		}
		else if (heap->type == Heap::Type::SKELETON) {
			title = TXT_SKELETON;
			info = TXT_REMAINS;
		}
		else if (heap->type == Heap::Type::CRYSTAL_CHEST) {
			title = TXT_CRYSTAL_CHEST;
			info = GameMath::format(TXT_INSIDE.c_str(), heap->peek()->Name().c_str());
		}
		else {
			title = TXT_LOCKED_CHEST;
			info = TXT_NEED_KEY;
		}

		fillFields(heap->image(), heap->glowing(), TITLE_COLOR, title, info);

	}
}

WndInfoItem::WndInfoItem(Item* item)
{
	int color = TITLE_COLOR;
	if (item->levelKnown) {
		if (item->Level() < 0 || item->isBroken()) {
			color = ItemSlot::DEGRADED;
		}
		else if (item->Level() > 0) {
			color = ItemSlot::UPGRADED;
		}
	}

	fillFields(item->Image(), item->glowing(), color, item->toString(), item->info());
}

const float WndTradeItem::GAP = 2;

const String WndTradeItem::TXT_SALE = "FOR SALE: %s - %dg";

const String WndTradeItem::TXT_BUY = "Buy for %dg";

const String WndTradeItem::TXT_SELL = "Sell for %dg";

const String WndTradeItem::TXT_SELL_1 = "Sell 1 for %dg";

const String WndTradeItem::TXT_SELL_ALL = "Sell all for %dg";

const String WndTradeItem::TXT_CANCEL = "Never mind";

const String WndTradeItem::TXT_SOLD = "You've sold your %s for %dg";

const String WndTradeItem::TXT_BOUGHT = "You've bought %s for %dg";

float WndTradeItem::createDescription(Item* item, boolean forSale)
{
	IconTitle* titlebar = new IconTitle();
	titlebar->icon(new ItemSprite(item->Image(), item->glowing()));
	titlebar->label(forSale ?
		GameMath::format(TXT_SALE.c_str(), item->toString(), price(item)) :
		item->toString());
	titlebar->setRect(0, 0, WIDTH, 0);
	add(titlebar);
	
	if (item->levelKnown) {
		if (item->Level() < 0) {
			titlebar->color(ItemSlot::DEGRADED);
		}
		else if (item->Level() > 0) {
			titlebar->color(item->isBroken() ? ItemSlot::WARNING : ItemSlot::UPGRADED);
		}
	}
	
	BitmapTextMultiline* info = PixelScene::createMultiline(item->info(), 6);
	info->maxWidth = WIDTH;
	info->measure();
	info->x = titlebar->left();
	info->y = titlebar->bottom() + GAP;
	add(info);
	
	return info->y + info->getHeight();
}

void WndTradeItem::sell(Item* item)
{
	Hero* hero = Dungeon::hero;
	
	if (item->isEquipped(hero) && !((EquipableItem*)item)->doUnequip(hero, false)) {
		return;
	}
	item->detachAll(hero->belongings->backpack);
	
	int price = item->price();
	
	Gold* g = new Gold(price);
	g->doPickUp(hero);
	GLog::i(TXT_SOLD.c_str(), item->Name().c_str(), price);
}

void WndTradeItem::sellOne(Item* item)
{
	if (item->Quantity() <= 1) {
		sell(item);
	}
	else {
	
		Hero* hero = Dungeon::hero;
	
		item = item->detach(hero->belongings->backpack);
		int price = item->price();
	
		// TODO: Check Memory Leak
		Gold* g = new Gold(price);
		g->doPickUp(hero);
		GLog::i(TXT_SOLD.c_str(), item->Name().c_str(), price);
	}
}

int WndTradeItem::price(Item* item)
{
	int price = item->price() * 5 * (Dungeon::depth / 5 + 1);
	if (Dungeon::hero->buff("RingOfHaggler::Haggling") != NULL && price >= 2) {
		price /= 2;
	}
	return price;
}

void WndTradeItem::buy(Heap* heap)
{
	Hero* hero = Dungeon::hero;
	Item* item = heap->pickUp();

	int price = this->price(item);
	Dungeon::gold -= price;

	GLog::i(TXT_BOUGHT.c_str(), item->Name().c_str(), price);

	if (!item->doPickUp(hero)) {
		Dungeon::level->drop(item, heap->pos)->sprite->drop();
	}
}

namespace{
	class RedButtonNewSell :public RedButton{
	public:
		WndTradeItem* wti;
		Item* item;
		RedButtonNewSell(WndTradeItem* w, Item* i, const std::string& txt)
			:RedButton(txt),
			wti(w),
			item(i){}

	protected:
		virtual void onClick()
		{
			wti->sell(item);
			wti->hide();
		}
	};
	class RedButtonNewSell1 :public RedButton{
	public:
		WndTradeItem* wti;
		Item* item;
		RedButtonNewSell1(WndTradeItem* w, Item* i, const std::string& txt)
			:RedButton(txt),
			wti(w),
			item(i){}

	protected:
		virtual void onClick()
		{
			wti->sellOne(item);
			wti->hide();
		}
	};
	class RedButtonNewSellAll :public RedButton{
	public:
		WndTradeItem* wti;
		Item* item;
		RedButtonNewSellAll(WndTradeItem* w, Item* i, const std::string& txt)
			:RedButton(txt),
			wti(w),
			item(i){}

	protected:
		virtual void onClick()
		{
			wti->sell(item);
			wti->hide();
		}
	};
	class RedButtonNewCancel :public RedButton{
	public:
		WndTradeItem* wti;
		RedButtonNewCancel(WndTradeItem* w, const std::string& txt)
			:RedButton(txt),
			wti(w){}

	protected:
		virtual void onClick()
		{
			wti->hide();
		}
	};
	class RedButtonNewBuy :public RedButton{
	public:
		WndTradeItem* wti;
		Heap* heap;
		RedButtonNewBuy(WndTradeItem* w, Heap* h, const std::string& txt)
			:RedButton(txt),
			wti(w),
			heap(h){}

	protected:
		virtual void onClick()
		{
			wti->buy(heap);
			wti->hide();
		}
	};
}
WndTradeItem::WndTradeItem(Item* item, WndBag* owner)
{
	this->owner = owner;
	
	float pos = createDescription(item, false);
	
	if (item->Quantity() == 1) {
	
		RedButton* btnSell = new RedButtonNewSell(this, item, GameMath::format(TXT_SELL.c_str(), item->price()));
		btnSell->setRect(0, pos + GAP, WIDTH, BTN_HEIGHT);
		add(btnSell);
	
		pos = btnSell->bottom();	
	}
	else {
	
		int priceAll = item->price();
		RedButton* btnSell1 = new RedButtonNewSell1(this, item, GameMath::format(TXT_SELL_1.c_str(), priceAll / item->Quantity()));
		btnSell1->setRect(0, pos + GAP, WIDTH, BTN_HEIGHT);
		add(btnSell1);

		RedButton* btnSellAll = new RedButtonNewSellAll(this, item, GameMath::format(TXT_SELL_ALL.c_str(), priceAll));
		btnSellAll->setRect(0, btnSell1->bottom() + GAP, WIDTH, BTN_HEIGHT);
		add(btnSellAll);
	
		pos = btnSellAll->bottom();
	
	}
	
	RedButton* btnCancel = new RedButtonNewCancel(this, TXT_CANCEL);
	btnCancel->setRect(0, pos + GAP, WIDTH, BTN_HEIGHT);
	add(btnCancel);
	
	resize(WIDTH, (int)btnCancel->bottom());
}

WndTradeItem::WndTradeItem(Heap* heap, bool canBuy)
{
	Item* item = heap->peek();
	
	float pos = createDescription(item, true);
	
	int price = this->price(item);
	
	if (canBuy) {
	
		RedButton* btnBuy = new RedButtonNewBuy(this, heap, GameMath::format(TXT_BUY.c_str(), price));
		btnBuy->setRect(0, pos + GAP, WIDTH, BTN_HEIGHT);
		btnBuy->enable(price <= Dungeon::gold);
		add(btnBuy);
	
		RedButton* btnCancel = new RedButtonNewCancel(this, TXT_CANCEL);
		btnCancel->setRect(0, btnBuy->bottom() + GAP, WIDTH, BTN_HEIGHT);
		add(btnCancel);
	
		resize(WIDTH, (int)btnCancel->bottom());
	
	}
	else {
	
		resize(WIDTH, (int)pos);
	
	}
}

void WndTradeItem::hide()
{
	Window::hide();

	if (owner != NULL) {
		owner->hide();
		delete owner;
		//Shopkeeper::sell();
	}

	delete this;
}
