#include "wndbag.h"
#include "belongings.h"
#include "pixeldungeon.h"
#include "pixelscene.h"
#include "bag.h"
#include "dungeon.h"
#include "hero.h"

WndBag::Mode WndBag::lastMode;
Bag* WndBag::lastBag;

WndBag::WndBag(Bag* bag, Listener* listener, Mode mode, const std::string& title)
{
	this->listener = listener;
	this->mode = mode;
	this->title = title;

	lastMode = mode;
	lastBag = bag;

	nCols = PixelDungeon::landscape() ? COLS_L : COLS_P;
	nRows = (Belongings::BACKPACK_SIZE + 4 + 1) / nCols + ((Belongings::BACKPACK_SIZE + 4 + 1) % nCols > 0 ? 1 : 0);

	int slotsWidth = SLOT_SIZE * nCols + SLOT_MARGIN * (nCols - 1);
	int slotsHeight = SLOT_SIZE * nRows + SLOT_MARGIN * (nRows - 1);

	BitmapText* txtTitle = PixelScene::createText(title.size() > 0 ? title : bag->Name(), 9);
	txtTitle->hardlight(TITLE_COLOR);
	txtTitle->measure();
	txtTitle->x = (int)(slotsWidth - txtTitle->getWidth()) / 2;
	txtTitle->y = (int)(TITLE_HEIGHT - txtTitle->getHeight()) / 2;
	Group::add(txtTitle);

	placeItems(bag);

	resize(slotsWidth, slotsHeight + TITLE_HEIGHT);

	Belongings* stuff = Dungeon::hero->belongings;
	//Bag[] bags = {
	//	stuff.backpack,
	//	stuff.getItem(SeedPouch.class),
	//	stuff.getItem(ScrollHolder.class),
	//	stuff.getItem(WandHolster.class),
	//	stuff.getItem(Keyring.class) };
	//
	//for (Bag b : bags) {
	//	if (b != null) {
	//		BagTab tab = new BagTab(b);
	//		tab.setSize(TAB_WIDTH, tabHeight());
	//		add(tab);
	//
	//		tab.select(b == bag);
	//	}
	//}
}

void WndBag::placeItems(Bag* container)
{
	// Equipped items
	Belongings* stuff = Dungeon::hero->belongings;
	//placeItem(stuff.weapon != null ? stuff.weapon : new Placeholder(ItemSpriteSheet.WEAPON));
	//placeItem(stuff.armor != null ? stuff.armor : new Placeholder(ItemSpriteSheet.ARMOR));
	//placeItem(stuff.ring1 != null ? stuff.ring1 : new Placeholder(ItemSpriteSheet.RING));
	//placeItem(stuff.ring2 != null ? stuff.ring2 : new Placeholder(ItemSpriteSheet.RING));
	//
	//boolean backpack = (container == Dungeon.hero.belongings.backpack);
	//if (!backpack) {
	//	count = nCols;
	//	col = 0;
	//	row = 1;
	//}
	//
	//// Items in the bag
	//for (Item item : container.items) {
	//	placeItem(item);
	//}
	//
	//// Free space
	//while (count - (backpack ? 4 : nCols) < container.size) {
	//	placeItem(null);
	//}
	//
	//// Gold in the backpack
	//if (container == Dungeon.hero.belongings.backpack) {
	//	row = nRows - 1;
	//	col = nCols - 1;
	//	placeItem(new Gold(Dungeon.gold));
	//}
}

WndBag* WndBag::seedPouch(Listener* listener, Mode mode, const String& title)
{
	SeedPouch* pouch = (SeedPouch*)Dungeon::hero->belongings->getItem("SeedPouch");
	return pouch != NULL ?
		new WndBag(pouch, listener, mode, title) :
		new WndBag(Dungeon::hero->belongings->backpack, listener, mode, title);
}

WndBag* WndBag::LastBag(Listener* listener, Mode mode, const String& title)
{
	if (mode == lastMode && lastBag != NULL &&
		Dungeon::hero->belongings->backpack->contains(lastBag)) {

		return new WndBag(lastBag, listener, mode, title);
	}
	else {
		return new WndBag(Dungeon::hero->belongings->backpack, listener, mode, title);
	}
}
