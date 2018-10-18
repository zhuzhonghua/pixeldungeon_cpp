#include "heap.h"
#include "simpleresource.h"
#include "itemsprite.h"
#include "item.h"
#include "mimic.h"
#include "glog.h"
#include "dungeon.h"
#include "level.h"
#include "wraith.h"
#include "cellemitter.h"
#include "speck.h"
#include "alphatweener.h"
#include "dewdrop.h"
#include "scroll.h"
#include "food.h"
#include "splash.h"
#include "util.h"
#include "statistics.h"
#include "charsprite.h"
#include "pixelparticle.h"
#include "burning.h"
#include "flameparticle.h"
#include "generator.h"
#include "hero.h"

const String Heap::TXT_MIMIC = "This is a mimic!";
const float Heap::FADE_TIME = 0.6f;
const String Heap::POS = "pos";
const String Heap::TYPE = "type";
const String Heap::ITEMS = "items";

Heap::Heap()
{
	type = Type::HEAP;
	pos = 0;
}

ItemSprite::Glowing* Heap::glowing() 
{
	return (type == Type::HEAP || type == Type::FOR_SALE) && items.size() > 0 ? items[0]->glowing() : NULL;
}

int Heap::image()
{
	switch (type) 
	{
	case HEAP:
	case FOR_SALE:
		return size() > 0 ? items[0]->Image() : 0;
	case CHEST:
	case MIMIC:
		return ItemSpriteSheet::CHEST;
	case LOCKED_CHEST:
		return ItemSpriteSheet::LOCKED_CHEST;
	case CRYSTAL_CHEST:
		return ItemSpriteSheet::CRYSTAL_CHEST;
	case TOMB:
		return ItemSpriteSheet::TOMB;
	case SKELETON:
		return ItemSpriteSheet::BONES;
	case HIDDEN:
		return ItemSpriteSheet::HIDDEN;
	default:
		return 0;
	}
}

void Heap::open(Hero* hero)
{
	switch (type) {
	case MIMIC:
		if (Mimic::spawnAt(pos, items) != NULL) {
			GLog::n(TXT_MIMIC.c_str());
			destroy();
		}
		else {
			type = Type::CHEST;
		}
		break;
	case TOMB:
		Wraith::spawnAround(hero->pos);
		break;
	case SKELETON:
		CellEmitter::center(pos)->start(Speck::factory(Speck::RATTLE), 0.1f, 3);
		for (int i = 0; i < items.size(); i++){
			Item* item = items[i];
		//for (Item item : items) {
			if (item->cursed) {
				if (Wraith::spawnAt(pos) == NULL) {
					hero->sprite->emitter()->burst(ShadowParticle::curse(), 6);
					hero->damage(hero->HP / 2, this->getClassName());
				}
				//Sample.INSTANCE.play(Assets.SND_CURSED);
				break;
			}
		}
		break;
	case HIDDEN:
		sprite->alpha(0);
		sprite->parent->add(new AlphaTweener(sprite, 1, FADE_TIME));
		break;
	default:
		break;
	}

	if (type != Type::MIMIC) {
		type = Type::HEAP;
		sprite->link();
		sprite->drop();
	}
}

void Heap::destroy()
{
	std::map<int, Heap*>::iterator itr = Dungeon::level->heaps.find(this->pos);
	Dungeon::level->heaps.erase(itr);

	if (sprite != NULL) {
		sprite->kill();
		delete sprite;
		sprite = NULL;
	}
	items.clear();

	delete this;
}

void Heap::restoreFromBundle(Bundle* bundle)
{
	pos = bundle->getInt(POS);
	//type = Type.valueOf(bundle.getString(TYPE));
	//items = new LinkedList<Item>((Collection< ? extends Item>) bundle.getCollection(ITEMS));
}

void Heap::storeInBundle(Bundle* bundle)
{
	pos = bundle->getInt(POS);
	//type = Type.valueOf(bundle.getString(TYPE));
	//items = new LinkedList<Item>((Collection< ? extends Item>) bundle.getCollection(ITEMS));
}

Item* Heap::pickUp()
{	
	Item* item = items[0];
	items.erase(items.begin());

	if (items.size() <= 0) {
		destroy();
	}
	else if (sprite != NULL) {
		sprite->view(image(), glowing());
	}

	return item;
}

void Heap::drop(Item* item)
{
	if (item->stackable) {

		std::string className = item->getClassName();
		for (int i = 0; i < items.size(); i++){
			Item* itm = items[i];
		//for (Item i : items) {
			//if (i.getClass() == c) {
			if (itm->getClassName() == className){
				itm->quantity += item->quantity;
				item = itm;
				break;
			}
		}
		for (std::vector<Item*>::iterator itr = items.begin();
			itr != items.end(); itr++)
		{
			if (*itr == item)
			{
				items.erase(itr);
				break;
			}
		}
	}

	if (dynamic_cast<Dewdrop*>(item)) {
		items.push_back(item);
	}
	else {
		items.insert(items.begin(), item);
	}

	if (sprite != NULL) {
		sprite->view(image(), glowing());
	}
}

void Heap::replace(Item* a, Item* b)
{
	std::vector<Item*>::iterator itr = std::find(items.begin(), items.end(), a);
	if (itr != items.end()){
		*itr = b;
	}
}

void Heap::burn()
{
	if (type == Type::MIMIC) {
		Mimic* m = Mimic::spawnAt(pos, items);
		if (m != NULL) {
			((Burning*)Buff::affect(m, "Burning"))->reignite(m);
			m->sprite->emitter()->burst(FlameParticle::FACTORY, 5);
			destroy();
		}
	}
	if (type != Type::HEAP) {
		return;
	}

	boolean burnt = false;
	boolean evaporated = false;

	//for (Item item : items.toArray(new Item[0])) {
	for (std::vector<Item*>::iterator itr = items.begin();
		itr != items.end(); ){
		Item* item = *itr;
		//if (item instanceof Scroll) {
		if (dynamic_cast<Scroll*>(item)){
			//items.remove(item);
			itr = items.erase(itr);
			burnt = true;
			continue;
		}
		else if (dynamic_cast<Dewdrop*>(item)) {
			itr = items.erase(itr);
			evaporated = true;
			continue;
		}
		else if (dynamic_cast<MysteryMeat*>(item)) {
			replace(item, ChargrilledMeat::cook((MysteryMeat*)item));
			burnt = true;
			continue;
		}

		itr++;
	}

	if (burnt || evaporated) {

		if (Dungeon::visible[pos]) {
			if (burnt) {
				burnFX(pos);
			}
			else {
				evaporateFX(pos);
			}
		}

		if (isEmpty()) {
			destroy();
		}
		else if (sprite != NULL) {
			sprite->view(image(), glowing());
		}

	}
}

void Heap::freeze()
{
	if (type == Type::MIMIC) {
		Mimic* m = Mimic::spawnAt(pos, items);
		if (m != NULL) {
			Buff::prolong(m, "Frost", Frost::duration(m) * Random::Float(1.0f, 1.5f));
			destroy();
		}
	}
	if (type != Type::HEAP) {
		return;
	}

	boolean frozen = false;

	std::vector<Item*> arr = items;
	for (std::vector<Item*>::iterator itr = items.begin();
		itr != items.end(); itr++){
		Item* item = *itr;
		if (dynamic_cast<MysteryMeat*>(item)){
			replace(item, FrozenCarpaccio::cook((MysteryMeat*)item));
			frozen = true;
		}
	}

	if (frozen) {
		if (isEmpty()) {
			destroy();
		}
		else if (sprite != NULL) {
			sprite->view(image(), glowing());
		}
	}
}

Item* Heap::transmute()
{
	CellEmitter::get(pos)->burst(Speck::factory(Speck::BUBBLE), 3);
	Splash::at(pos, 0xFFFFFF, 3);

	std::vector<float> chances(items.size());
	int count = 0;

	int index = 0;

	for (int i = 0;i<items.size();i++){

		Item* item = items[i];
		if (dynamic_cast<Plant::Seed*>(item)){
			count += item->quantity;
			chances[index++] = item->quantity;
		}
		else {
			count = 0;
			break;
		}
	}

	if (count >= SEEDS_TO_POTION) {

		CellEmitter::get(pos)->burst(Speck::factory(Speck::WOOL), 6);
		//Sample.INSTANCE.play(Assets.SND_PUFF);

		if (Random::Int(count) == 0) {

			CellEmitter::center(pos)->burst(Speck::factory(Speck::EVOKE), 3);

			destroy();

			Statistics::potionsCooked++;
			Badges::validatePotionsCooked();

			return Generator::random(Generator::Category::POTION);

		}
		else {

			Plant::Seed* proto = (Plant::Seed*)items[Random::chances(chances)];
			std::string itemClass = proto->alchemyClass;

			destroy();

			Statistics::potionsCooked++;
			Badges::validatePotionsCooked();

			if (itemClass.size() <= 0) {
				return Generator::random(Generator::Category::POTION);
			}
			else {
				return FactoryItem::Create(itemClass);// itemClass.newInstance();
			}
		}

	}
	else {
		return NULL;
	}
}

void Heap::burnFX(int pos)
{
	CellEmitter::get(pos)->burst(ElmoParticle::factory(), 6);
	//Sample.INSTANCE.play(Assets.SND_BURNING);
}

void Heap::evaporateFX(int pos)
{
	CellEmitter::get(pos)->burst(Speck::factory(Speck::STEAM), 5);
}
