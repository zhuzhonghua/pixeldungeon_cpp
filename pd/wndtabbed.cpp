#include "wndtabbed.h"
#include "simpleresource.h"
#include "pixelscene.h"
#include "game.h"
#include "dungeon.h"
#include "hero.h"
#include "redbutton.h"
#include "statistics.h"
#include "buffindicator.h"
#include "gamescene.h"
#include "pixeldungeon.h"
#include "scrollpane.h"
#include "potion.h"
#include "scroll.h"

WndTabbed::Tab::Tab(WndTabbed* wnd)
{
	this->wnd = wnd;
	bg = NULL;
	init();
}

void WndTabbed::Tab::layout()
{
	Button::layout();

	if (bg != NULL) 
	{
		bg->x = _x;
		bg->y = _y;
		bg->size(_width, _height);
	}
}

void WndTabbed::Tab::select(bool value)
{
	active = !(selected = value);

	if (bg != NULL) 
	{
		remove(bg);
	}

	bg = Chrome::get(selected ? Chrome::Type::TAB_SELECTED : Chrome::Type::TAB_UNSELECTED);
	addToBack(bg);

	layout();
}

void WndTabbed::Tab::onClick()
{
	//Sample.INSTANCE.play(Assets.SND_CLICK, 0.7f, 0.7f, 1.2f);
	wnd->onClick(this);
}

WndTabbed::LabeledTab::LabeledTab(WndTabbed* wnd, const std::string& label) 
:Tab(wnd)
{
	init();
	btLabel->text(label);
	btLabel->measure();
}

void WndTabbed::LabeledTab::createChildren()
{
	btLabel = PixelScene::createText(9);
	add(btLabel);
}

void WndTabbed::LabeledTab::layout()
{
	Tab::layout();

	btLabel->x = PixelScene::align(_x + (_width - btLabel->Width()) / 2);
	btLabel->y = PixelScene::align(_y + (_height - btLabel->baseLine()) / 2) - 1;
	if (!selected) 
	{
		btLabel->y -= 2;
	}
}

void WndTabbed::LabeledTab::select(bool value)
{
	Tab::select(value);
	btLabel->am = selected ? 1.0f : 0.6f;
}

void WndTabbed::onClick(Tab* tab)
{
	select(tab);
}

void WndTabbed::resize(int w, int h)
{
	this->width = w;
	this->height = h;

	chrome->size(
		width + chrome->marginHor(),
		height + chrome->marginVer());

	cameraf->resize((int)chrome->width, (int)(chrome->marginTop() + height + tabHeight()));
	cameraf->x = (int)(Game::width - cameraf->screenWidth()) / 2;
	cameraf->y = (int)(Game::height - cameraf->screenHeight()) / 2;

	shadow->boxRect(
		cameraf->x / cameraf->zoomf,
		cameraf->y / cameraf->zoomf,
		chrome->Width(), chrome->height);

	for (int i = 0; i < tabs.size();i++)
	{
		remove(tabs[i]);
	}

	std::vector<Tab*> newTabs = this->tabs;
	
	this->tabs.clear();

	for (int i = 0; i < newTabs.size();i++)
	{
		add(newTabs[i]);
	}
}

void WndTabbed::select(Tab* tab)
{
	if (tab != selected) 
	{
		for (int i = 0; i < tabs.size();i++)
		{
			if (tabs[i] == selected)
			{
				tabs[i]->select(false);
			}
			else if (tabs[i] == tab)
			{
				tabs[i]->select(true);
			}
		}

		selected = tab;
	}
}

void WndTabbed::select(int index)
{
	select(tabs[index]);
}

WndTabbed::Tab* WndTabbed::add(Tab* tab)
{
	tab->setPos(tabs.size() == 0 ?
		-chrome->marginLeft() + 1 :
		tabs[tabs.size() - 1]->right(), height);
	tab->select(false);

	Window::add(tab);

	tabs.push_back(tab);

	return tab;
}

WndTabbed::WndTabbed()
:Window(0, 0, Chrome::get(Chrome::Type::TAB_SET))
{

}

const String WndHero::TXT_STATS = BPT::getText("lang.Stats");

const String WndHero::TXT_BUFFS = BPT::getText("lang.Buffs");

const String WndHero::TXT_EXP = BPT::getText("lang.Experience");

const String WndHero::TXT_STR = BPT::getText("lang.Strength");

const String WndHero::TXT_HEALTH = BPT::getText("lang.Health");

const String WndHero::TXT_GOLD = BPT::getText("lang.Gold_Collected");

const String WndHero::TXT_DEPTH = BPT::getText("lang.Maximum_Depth");

void WndHero::StatsTab::statSlot(const String& label, const String& value)
{
	BitmapText* txt = PixelScene::createText(label, 8);
	txt->y = pos;
	add(txt);

	txt = PixelScene::createText(value, 8);
	txt->measure();
	txt->x = PixelScene::align(WIDTH * 0.65f);
	txt->y = pos;
	add(txt);

	pos += GAP + txt->baseLine();
}

void WndHero::StatsTab::statSlot(const String& label, int value)
{
	std::stringstream s;
	s << value;
	statSlot(label, s.str());
}

const String WndHero::StatsTab::TXT_TITLE = BPT::getText("lang.wndhero_statstab_title");

const String WndHero::StatsTab::TXT_CATALOGUS = BPT::getText("lang.Catalogus");

const String WndHero::StatsTab::TXT_JOURNAL = BPT::getText("lang.Journal");

namespace{
	class RedButtonCatalogus :public RedButton{
	public:
		WndHero* wst;
		RedButtonCatalogus(WndHero* w, const std::string& str)
			:RedButton(str),
			wst(w){}
	protected:
		virtual void onClick()
		{
			wst->hide();
			GameScene::show(new WndCatalogus());
		}
	};
	class RedButtonJournal :public RedButton{
	public:
		WndHero* wst;
		RedButtonJournal(WndHero* w, const std::string& str)
			:RedButton(str),
			wst(w){}
	protected:
		virtual void onClick()
		{
			wst->hide();
			GameScene::show(new WndJournal());
		}
	};
}
WndHero::StatsTab::StatsTab(WndHero* w)
:wh(w)
{
	Hero* hero = Dungeon::hero;

	BitmapText* title = PixelScene::createText(
		GameMath::format(TXT_TITLE.c_str(), hero->lvl, hero->className().c_str()), 9);
	title->hardlight(TITLE_COLOR);
	title->measure();
	add(title);

	RedButton* btnCatalogus = new RedButtonCatalogus(wh, TXT_CATALOGUS);

	btnCatalogus->setRect(0, title->y + title->Height(), btnCatalogus->reqWidth() + 2, btnCatalogus->reqHeight() + 2);
	add(btnCatalogus);

	RedButton* btnJournal = new RedButtonJournal(wh, TXT_JOURNAL);
	btnJournal->setRect(
		btnCatalogus->right() + 1, btnCatalogus->top(),
		btnJournal->reqWidth() + 2, btnJournal->reqHeight() + 2);
	add(btnJournal);

	pos = btnCatalogus->bottom() + GAP;

	statSlot(TXT_STR, hero->sTR());
	statSlot(TXT_HEALTH, hero->HP + "/" + hero->HT);
	statSlot(TXT_EXP, hero->exp + "/" + hero->maxExp());

	pos += GAP;

	statSlot(TXT_GOLD, Statistics::goldCollected);
	statSlot(TXT_DEPTH, Statistics::deepestFloor);

	pos += GAP;
}

void WndHero::BuffsTab::buffSlot(Buff* buff)
{
	int index = buff->icon();

	if (index != BuffIndicator::NONE) {

		Image* icon = new Image(wh->icons);
		icon->frame(wh->film->get(index));
		icon->y = pos;
		add(icon);

		BitmapText* txt = PixelScene::createText(buff->toString(), 8);
		txt->x = icon->width + GAP;
		txt->y = pos + (int)(icon->height - txt->baseLine()) / 2;
		add(txt);

		pos += GAP + icon->height;
	}
}

WndHero::BuffsTab::BuffsTab(WndHero* w)
:wh(w)
{
	for (std::set<Buff*>::iterator itr = Dungeon::hero->buffs.begin();
		itr != Dungeon::hero->buffs.end(); itr++)
	{
		buffSlot(*itr);
	}
}

const String WndCatalogus::TXT_POTIONS = BPT::getText("lang.Potions");

const String WndCatalogus::TXT_SCROLLS = BPT::getText("lang.Scrolls");

const String WndCatalogus::TXT_TITLE = BPT::getText("lang.Catalogus");

boolean WndCatalogus::showPotions = true;

void WndCatalogus::updateList()
{
	txtTitle->text(GameMath::format(TXT_TITLE.c_str(), showPotions ? TXT_POTIONS.c_str() : TXT_SCROLLS.c_str()));
	txtTitle->measure();
	txtTitle->x = PixelScene::align(PixelScene::uiCamera, (width - txtTitle->Width()) / 2);

	for (ArrayList<ListItem*>::iterator itr = items.begin();
		itr != items.end(); itr++)
	{
		delete *itr;
	}
	items.clear();

	Component* content = list->Content();
	content->clear();
	list->scrollTo(0, 0);
	
	float pos = 0;
	std::set<std::string> known = showPotions ? Potion::getKnown() : Scroll::getKnown();

	for (std::set<std::string>::iterator itr = known.begin(); itr != known.end(); itr++)
	{
		std::string itemClass = *itr;
		ListItem* item = new ListItem(itemClass);
		item->setRect(0, pos, width, ITEM_HEIGHT);
		content->add(item);
		items.push_back(item);

		pos += item->height();
	}
	
	std::set<std::string> unknown = showPotions ? Potion::getUnknown() : Scroll::getUnknown();
	for (std::set<std::string>::iterator itr = unknown.begin(); itr != unknown.end(); itr++)
	{
		std::string itemClass = *itr;
		ListItem* item = new ListItem(itemClass);
		item->setRect(0, pos, width, ITEM_HEIGHT);
		content->add(item);
		items.push_back(item);

		pos += item->height();
	}

	content->setSize(width, pos);
	list->setSize(list->width(), list->height());
}

namespace{
	class ComponentNew :public Component{
	public:
		WndCatalogus* wcl;
		ComponentNew(WndCatalogus* w)
			:wcl(w){}
	public:
		virtual void onClick(float x, float y)
		{
			for (ArrayList<WndCatalogus::ListItem*>::iterator itr = wcl->items.begin();
				itr != wcl->items.end(); itr++)
			{
				if ((*itr)->onClick(x, y)){
					break;
				}
			}
		}
	};

	class LabeledTabPotion :public WndCatalogus::LabeledTab{
	public:
		WndCatalogus* wcl;
		LabeledTabPotion(WndCatalogus* w, const std::string& l)
			:wcl(w),
			WndCatalogus::LabeledTab(w, l){}

	protected:
		virtual void select(boolean value) {
			WndCatalogus::LabeledTab::select(value);
			WndCatalogus::showPotions = value;
			wcl->updateList();
		};
	};

	class LabeledTabScroll :public WndCatalogus::LabeledTab{
	public:
		WndCatalogus* wcl;
		LabeledTabScroll(WndCatalogus* w, const std::string& l)
			:wcl(w),
			WndCatalogus::LabeledTab(w, l){}

	protected:
		virtual void select(boolean value) {
			WndCatalogus::LabeledTab::select(value);
			WndCatalogus::showPotions = !value;
			wcl->updateList();
		};
	};
}
WndCatalogus::WndCatalogus()
{
	if (PixelDungeon::landscape()) {
		resize(WIDTH_L, HEIGHT_L);
	}
	else {
		resize(WIDTH_P, HEIGHT_P);
	}

	txtTitle = PixelScene::createText(TXT_TITLE, 9);
	txtTitle->hardlight(Window::TITLE_COLOR);
	txtTitle->measure();
	Window::add(txtTitle);

	list = new ScrollPane(new ComponentNew(this));
	Window::add(list);
	list->setRect(0, txtTitle->Height(), width, height - txtTitle->Height());

	boolean showPotions = WndCatalogus::showPotions;
	std::vector<Tab*> tabs;
	tabs.push_back(new LabeledTabPotion(this, TXT_POTIONS));
	tabs.push_back(new LabeledTabScroll(this, TXT_SCROLLS));

	for (int i = 0; i < tabs.size(); i++)
	{
		Tab* tab = tabs[i];
		tab->setSize(TAB_WIDTH, tabHeight());
		add(tab);
	}

	select(showPotions ? 0 : 1);
}

WndCatalogus::ListItem::ListItem(const std::string& cl)
{
	init();

	item = FactoryItem::Create(cl);
	if (identified = item->isIdentified()) {
		sprite->view(item->Image(), NULL);
		label->text(item->Name());
	}
	else {
		sprite->view(127, NULL);
		label->text(item->trueName());
		label->hardlight(0xCCCCCC);
	}
}

boolean WndCatalogus::ListItem::onClick(float x, float y)
{
	if (identified && inside(x, y)) {
		GameScene::show(new WndInfoItem(item));
		return true;
	}
	else {
		return false;
	}
}

void WndCatalogus::ListItem::createChildren()
{
	sprite = new ItemSprite();
	add(sprite);

	label = PixelScene::createText(8);
	add(label);
}

void WndCatalogus::ListItem::layout()
{
	sprite->y = PixelScene::align(_y + (_height - sprite->height) / 2);

	label->x = sprite->x + sprite->width;
	label->y = PixelScene::align(_y + (_height - label->baseLine()) / 2);
}
