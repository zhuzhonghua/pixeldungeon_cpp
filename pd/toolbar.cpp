#include "toolbar.h"
#include "image.h"
#include "dungeontilemap.h"
#include "item.h"
#include "game.h"
#include "util.h"
#include "dungeon.h"
#include "hero.h"
#include "gamescene.h"
#include "wndmessage.h"
#include "heap.h"
#include "belongings.h"
#include "quickslot.h"
#include "bpt.h"
#include "wndtitledmessage.h"
#include "healthindicator.h"

void Toolbar::Tool::createChildren()
{
	base = new Image(Assets::TOOLBAR);
	add(base);
}

void Toolbar::Tool::layout()
{
	Component::layout();

	base->x = _x;
	base->y = _y;
}

void Toolbar::Tool::onTouchDown()
{
	base->brightness(1.4f);
}

void Toolbar::Tool::onTouchUp()
{
	if (active) 
	{
		base->resetColor();
	}
	else 
	{
		base->tint(BGCOLOR, 0.7f);
	}
}

Toolbar::Tool::Tool(int x, int y, int width, int height)
{
	init();

	base->frame(x, y, width, height);

	this->_width = width;
	this->_height = height;
}

void Toolbar::Tool::enable(bool value)
{
	if (value != active) 
	{
		if (value) 
		{
			base->resetColor();
		}
		else 
		{
			base->tint(BGCOLOR, 0.7f);
		}
		active = value;
	}
}

const float Toolbar::PickedUpItem::DISTANCE = DungeonTilemap::SIZE;
const float Toolbar::PickedUpItem::DURATION = 0.2f;

Toolbar::PickedUpItem::PickedUpItem()
{
	originToCenter();

	active = visible = false;
}

void Toolbar::PickedUpItem::reset(Item* item, float dstX, float dstY)
{
	view(item->Image(), item->glowing());

	active = visible = true;

	this->dstX = dstX - ItemSprite::SIZE / 2;
	this->dstY = dstY - ItemSprite::SIZE / 2;
	left = DURATION;

	x = this->dstX - DISTANCE;
	y = this->dstY - DISTANCE;
	alpha(1);
}

void Toolbar::PickedUpItem::update()
{
	ItemSprite::update();

	if ((left -= Game::elapsed) <= 0) 
	{

		visible = active = false;

	}
	else 
	{
		float p = left / DURATION;
		GameMath::PointFSet(&scale, (float)std::sqrt(p));
		float offset = DISTANCE * p;
		x = dstX - offset;
		y = dstY - offset;
	}
}

namespace{
	class CellSelectorNew :public CellSelector::Listener{
	public:
		virtual void onSelect(int cell){
			if (cell == -1) {
				return;
			}

			if (cell < 0 || cell > Level::LENGTH || (!Dungeon::level->visited[cell] && !Dungeon::level->mapped[cell])) {
				GameScene::show(new WndMessage(BPT::getText("lang.dont_know_what_there")));
				return;
			}

			if (!Dungeon::visible[cell]) {
				GameScene::show(new WndInfoCell(cell));
				return;
			}

			if (cell == Dungeon::hero->pos) {
				GameScene::show(new WndHero());
				return;
			}

			Mob* mob = (Mob*)Actor::findChar(cell);
			if (mob != NULL) {
				GameScene::show(new WndInfoMob(mob));
				return;
			}

			Heap* heap = Dungeon::level->heaps.get(cell);
			if (heap != NULL && heap->type != Heap::Type::HIDDEN) {
				if (heap->type == Heap::Type::FOR_SALE && heap->size() == 1 && heap->peek()->price() > 0) {
					GameScene::show(new WndTradeItem(heap, false));
				}
				else {
					GameScene::show(new WndInfoItem(heap));
				}
				return;
			}

			Plant* plant = Dungeon::level->plants.get(cell);
			if (plant != NULL) {
				//GameScene::show(new WndInfoPlant(plant));
				return;
			}

			GameScene::show(new WndInfoCell(cell));
		}

		virtual String prompt() {
			return BPT::getText("lang.Select_a_cell_to_examine");
		}
	};
}
CellSelector::Listener* Toolbar::informer = new CellSelectorNew();

void Toolbar::update()
{
	Component::update();

	if (lastEnabled != Dungeon::hero->ready) {
		lastEnabled = Dungeon::hero->ready;

		for (int i = 0; i < _members.size(); i++)
		{
			Gizmo* tool = _members[i];
			if (dynamic_cast<Tool*>(tool))
			{
				((Tool*)tool)->enable(lastEnabled);
			}
		}
	}

	if (!Dungeon::hero->isAlive()) {
		btnInventory->enable(true);
	}
}

Toolbar* Toolbar::instance;

namespace{
	class ToolWait :public Toolbar::Tool{
	public:
		ToolWait(int x, int y, int width, int height)
			:Toolbar::Tool(x,y,width,height){}
	protected:
		virtual void onClick(){
			Dungeon::hero->rest(false);
		}
		virtual boolean onLongClick(){
			Dungeon::hero->rest(true);
			return true;
		}
	};
	class ToolSearch :public Toolbar::Tool{
	public:
		ToolSearch(int x, int y, int width, int height)
			:Toolbar::Tool(x, y, width, height){}
	protected:
		virtual void onClick(){
			Dungeon::hero->search(true);
		}
	};
	class ToolInfo :public Toolbar::Tool{
	public:
		ToolInfo(int x, int y, int width, int height)
			:Toolbar::Tool(x, y, width, height){}
	protected:
		virtual void onClick(){
			GameScene::selectCell(Toolbar::informer);
		}
	};
	class ToolInventory :public Toolbar::Tool{
	private:
		GoldIndicator* gold;
	public:
		ToolInventory(int x, int y, int width, int height)
			:Toolbar::Tool(x, y, width, height){ init(); }
	protected:
		virtual void onClick(){
			GameScene::show(new WndBag(Dungeon::hero->belongings->backpack, NULL, WndBag::Mode::ALL, NULL));
		}
		virtual boolean onLongClick(){
			GameScene::show(new WndCatalogus());
			return true;
		}
		virtual void createChildren(){
			gold = new GoldIndicator();
			add(gold);
		}
		virtual void layout(){
			Toolbar::Tool::layout();
			gold->fill(this);
		}
	};
}
void Toolbar::createChildren()
{
	add(btnWait = new ToolWait(0, 7, 20, 25));	
	add(btnSearch = new ToolSearch(20, 7, 20, 25));	
	add(btnInfo = new ToolInfo(40, 7, 21, 25));	
	add(btnInventory = new ToolInventory(60, 7, 23, 25));
	
	add(btnQuick1 = new QuickslotTool(83, 7, 22, 25, true));
	add(btnQuick2 = new QuickslotTool(83, 7, 22, 25, false));
	btnQuick2->visible = (QuickSlot::secondaryValue != NULL);
	
	add(pickedUp = new PickedUpItem());
}

void Toolbar::layout()
{
	btnWait->setPos(_x, _y);
	btnSearch->setPos(btnWait->right(), _y);
	btnInfo->setPos(btnSearch->right(), _y);
	btnQuick1->setPos(_width - btnQuick1->width(), _y);
	if (btnQuick2->visible) {
		btnQuick2->setPos(btnQuick1->left() - btnQuick2->width(), _y);
		btnInventory->setPos(btnQuick2->left() - btnInventory->width(), _y);
	}
	else {
		btnInventory->setPos(btnQuick1->left() - btnInventory->width(), _y);
	}
}

Toolbar::Toolbar()
{
	init();

	instance = this;
	lastEnabled = true;
	_height = btnInventory->height();
}

void Toolbar::pickup(Item* item)
{
	pickedUp->reset(item,
		btnInventory->centerX(),
		btnInventory->centerY());
}

bool Toolbar::secondQuickslot()
{
	return instance->btnQuick2->visible;
}

void Toolbar::secondQuickslot(boolean value)
{
	instance->btnQuick2->visible =
		instance->btnQuick2->active =
		value;
	instance->layout();
}

Toolbar::QuickslotTool::QuickslotTool(int x, int y, int width, int height, boolean primary)
:Toolbar::Tool(x, y, width, height)
{
	init();

	if (primary) {
		slot->primary();
	}
	else {
		slot->secondary();
	}
}

void Toolbar::QuickslotTool::enable(boolean value)
{
	slot->enable(value);
	Toolbar::Tool::enable(value);
}

void Toolbar::QuickslotTool::createChildren()
{
	slot = new QuickSlot();
	add(slot);
}

void Toolbar::QuickslotTool::layout()
{
	Toolbar::Tool::layout();
	slot->setRect(_x + 1, _y + 2, _width - 2, _height - 2);
}
