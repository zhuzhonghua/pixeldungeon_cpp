#include "wand.h"
#include "simpleresource.h"
#include "glog.h"
#include "util.h"
#include "dungeon.h"
#include "char.h"
#include "charsprite.h"
#include "alphatweener.h"
#include "speck.h"
#include "camera.h"
#include "level.h"
#include "mobsprite.h"
#include "belongings.h"
#include "gamescene.h"
#include "scroll.h"
#include "hero.h"

const std::string Wand::AC_ZAP = "ZAP";

const std::string Wand::UNFAMILIRIARITY = "unfamiliarity";
const std::string Wand::MAX_CHARGES = "maxCharges";
const std::string Wand::CUR_CHARGES = "curCharges";
const std::string Wand::CUR_CHARGE_KNOWN = "curChargeKnown";

Wand::Wand()
{
	curChargeKnown = false;
	
	usagesToKnow = USAGES_TO_KNOW;
	hitChars = true;

	defaultAction = AC_ZAP;

	init();
	//image = handler->image(this);
	//wood = handler->label(this);
}

void Wand::initWoods()
{
	handler = new ItemStatusHandler(wands, woods, images);
}

void Wand::save(Bundle* bundle)
{
	handler->save(bundle);
}

void Wand::restore(Bundle* bundle)
{
	handler = new ItemStatusHandler(wands, woods, images, bundle);
}

void Wand::actions(Hero* hero, std::vector<std::string>& actions)
{
	KindOfWeapon::actions(hero, actions);
	if (curCharges > 0 || !curChargeKnown) 
	{
		actions.push_back(AC_ZAP);
	}
	if (hero->heroClass.type() != HeroClass::E_MAGE) 
	{
		for (std::vector<std::string>::iterator itr = actions.begin();
			itr != actions.end();)
		{
			if (itr->compare(AC_EQUIP) == 0)
			{
				itr = actions.erase(itr);
			}
			else if (itr->compare(AC_UNEQUIP) == 0)
			{
				itr = actions.erase(itr);
			}
			else
			{
				itr++;
			}
		}
	}
}

bool Wand::doUnequip(Hero* hero, bool collect, bool single)
{
	onDetach();
	return KindOfWeapon::doUnequip(hero, collect, single);
}

void Wand::activate(Hero* hero) {
	charge(hero);
}

void Wand::execute(Hero* hero, const std::string& action)
{
	if (action.compare(AC_ZAP) == 0) 
	{
		curUser = hero;
		curItem = this;
		//GameScene::selectCell(zapper);

	}
	else 
	{
		KindOfWeapon::execute(hero, action);
	}
}

bool Wand::collect(Bag* container)
{
	//if (KindOfWeapon::collect(container)) 
	//{
	//	if (container.owner != null) {
	//		charge(container.owner);
	//	}
	//	return true;
	//}
	//else {
		return false;
	//}
}

void Wand::charge(Char* owner)
{
	if (charger == NULL) 
	{
		(charger = new Charger(this))->attachTo(owner);
	}
}

int Wand::power()
{
	int eLevel = effectiveLevel();
	//if (charger != NULL) 
	//{
	//	Power power = charger.target.buff(Power.class);
	//	return power == null ? eLevel : Math.max(eLevel + power.level, 0);
	//}
	//else {
		return eLevel;
	//}
}

void Wand::setKnown()
{
	//if (!isKnown()) {
	//	handler.know(this);
	//}
	//
	//Badges.validateAllWandsIdentified();
}

Item* Wand::identify()
{
	setKnown();
	curChargeKnown = true;
	KindOfWeapon::identify();

	updateQuickslot();

	return this;
}

std::string Wand::toString()
{
	//StringBuilder sb = new StringBuilder(super.toString());
	//
	//String status = status();
	//if (status != null) {
	//	sb.append(" (" + status + ")");
	//}
	//
	//if (isBroken()) {
	//	sb.insert(0, "broken ");
	//}
	//
	//return sb.toString();
	return "";
}

std::string Wand::info()
{
	//StringBuilder info = new StringBuilder(isKnown() ? desc() : String.format(TXT_WOOD, wood));
	//if (Dungeon.hero.heroClass == HeroClass.MAGE) {
	//	info.append("\n\n");
	//	if (levelKnown) {
	//		int min = min();
	//		info.append(String.format(TXT_DAMAGE, min + (max() - min) / 2));
	//	}
	//	else {
	//		info.append(String.format(TXT_WEAPON));
	//	}
	//}
	//return info.toString();
	return "";
}

bool Wand::isIdentified()
{
	return KindOfWeapon::isIdentified() && isKnown() && curChargeKnown;
}

std::string Wand::status()
{
	//if (levelKnown) {
	//	return std::string((curChargeKnown ? curCharges : "?")) + "/" + maxCharges;
	//}
	//else {
	//	return NULL;
	//}
	return "";
}

Item* Wand::upgrade()
{
	KindOfWeapon::upgrade();

	updateLevel();
	curCharges = std::min(curCharges + 1, maxCharges);
	updateQuickslot();

	return this;
}

Item* Wand::degrade()
{
	KindOfWeapon::degrade();

	updateLevel();
	updateQuickslot();

	return this;
}

Item* Wand::random()
{
	if (Random::Float() < 0.5f) {
		upgrade();
		if (Random::Float() < 0.15f) {
			upgrade();
		}
	}

	return this;
}

bool Wand::allKnown()
{
	//return handler->known()->size() == wands.size();
	return true;
}

void Wand::storeInBundle(Bundle* bundle)
{
	KindOfWeapon::storeInBundle(bundle);
	bundle->put(UNFAMILIRIARITY, usagesToKnow);
	bundle->put(MAX_CHARGES, maxCharges);
	bundle->put(CUR_CHARGES, curCharges);
	bundle->put(CUR_CHARGE_KNOWN, curChargeKnown);
}

void Wand::restoreFromBundle(Bundle* bundle)
{
	KindOfWeapon::restoreFromBundle(bundle);
	if ((usagesToKnow = bundle->getInt(UNFAMILIRIARITY)) == 0) 
	{
		usagesToKnow = USAGES_TO_KNOW;
	}
	maxCharges = bundle->getInt(MAX_CHARGES);
	curCharges = bundle->getInt(CUR_CHARGES);
	curChargeKnown = bundle->getBoolean(CUR_CHARGE_KNOWN);
}

namespace{
	class CellSelectorListenerNew :public CellSelector::Listener{
	public:
		virtual std::string prompt() {
			return "Choose direction to zap";
		}
		Wand* w;
		CellSelectorListenerNew(Wand* wa) :w(wa){}
		virtual void onSelect(int target)
		{
			if (target != NULL) 
			{

				//if (target == w->curUser->pos) {
				//	GLog::i(TXT_SELF_TARGET.c_str());
				//	return;
				//}
				//
				//Wand* curWand = (Wand*)Wand::curItem;
				//
				//curWand->setKnown();
				//
				//final int cell = Ballistica.cast(curUser.pos, target, true, curWand.hitChars);
				//curUser.sprite.zap(cell);
				//
				//QuickSlot.target(curItem, Actor.findChar(cell));
				//
				//if (curWand.curCharges > 0) {
				//
				//	curUser.busy();
				//
				//	curWand.fx(cell, new Callback(){
				//		@Override
				//		public void call() {
				//			curWand.onZap(cell);
				//			curWand.wandUsed();
				//		}
				//	});
				//
				//	Invisibility.dispel();
				//
				//}
				//else {
				//
				//	curUser.spendAndNext(TIME_TO_ZAP);
				//	GLog.w(TXT_FIZZLES);
				//	curWand.levelKnown = true;
				//
				//	curWand.updateQuickslot();
				//}

			}
		}
	};
}
CellSelector::Listener* Wand::zapper;

void Wand::init()
{
	maxCharges = initialCharges();
}

bool Wand::isKnown()
{
	//return handler->isKnown(this);
	return true;
}

void Wand::updateLevel()
{
	maxCharges = std::min(initialCharges() + Level(), 9);
	curCharges = std::min(curCharges, maxCharges);
}

void Wand::fx(int cell, Callback* callback)
{
	//MagicMissile.blueLight(curUser.sprite.parent, curUser.pos, cell, callback);
	//Sample.INSTANCE.play(Assets.SND_ZAP);
}

void Wand::wandUsed()
{
	curCharges--;
	if (!isIdentified() && --usagesToKnow <= 0) {
		identify();
		GLog::w(TXT_IDENTIFY.c_str(), Name());
	}
	else {
		updateQuickslot();
	}

	use();

	curUser->spendAndNext(TIME_TO_ZAP);
}

const std::string Wand::TXT_WOOD = "This thin %s wand is warm to the touch. Who knows what it will do when used?";
const std::string Wand::TXT_DAMAGE = "When this wand is used as a melee weapon, its average damage is %d points per hit.";
const std::string Wand::TXT_WEAPON = "You can use this wand as a melee weapon.";

const std::string Wand::TXT_FIZZLES = "your wand fizzles; it must be out of charges for now";
const std::string Wand::TXT_SELF_TARGET = "You can't target yourself";

const std::string Wand::TXT_IDENTIFY = "You are now familiar enough with your %s.";

const float Wand::TIME_TO_ZAP = 1.0f;

const std::string Wand::_woods[] =
{ "holly", "yew", "ebony", "cherry", "teak", "rowan", "willow", "mahogany", "bamboo", "purpleheart", "oak", "birch" };
std::vector<std::string> Wand::woods(_woods, _woods+sizeof(_woods)/sizeof(std::string));

const int Wand::_images[] = {
	ItemSpriteSheet::WAND_HOLLY,
	ItemSpriteSheet::WAND_YEW,
	ItemSpriteSheet::WAND_EBONY,
	ItemSpriteSheet::WAND_CHERRY,
	ItemSpriteSheet::WAND_TEAK,
	ItemSpriteSheet::WAND_ROWAN,
	ItemSpriteSheet::WAND_WILLOW,
	ItemSpriteSheet::WAND_MAHOGANY,
	ItemSpriteSheet::WAND_BAMBOO,
	ItemSpriteSheet::WAND_PURPLEHEART,
	ItemSpriteSheet::WAND_OAK,
	ItemSpriteSheet::WAND_BIRCH };

std::vector<int> Wand::images(_images, _images + sizeof(_images) / sizeof(int));

std::string Wand::_wands[] = {
	"WandOfTeleportation",
	"WandOfSlowness",
	"WandOfFirebolt",
	"WandOfPoison",
	"WandOfRegrowth",
	"WandOfBlink",
	"WandOfLightning",
	"WandOfAmok",
	"WandOfReach",
	"WandOfFlock",
	"WandOfDisintegration",
	"WandOfAvalanche"
};
std::vector<std::string> Wand::wands(_wands, _wands + sizeof(_wands) / sizeof(std::string));

ItemStatusHandler* Wand::handler;

bool Wand::Charger::act()
{
	if (w->curCharges < w->maxCharges) 
	{
		w->curCharges++;
		w->updateQuickslot();
	}

	delay();

	return true;
}

void Wand::Charger::delay()
{
	float time2charge = ((Hero*)target)->heroClass.type() == HeroClass::E_MAGE ?
		TIME_TO_CHARGE / (float)std::sqrt(1 + w->effectiveLevel()) :
		TIME_TO_CHARGE;
	spend(time2charge);
}

const float Wand::Charger::TIME_TO_CHARGE = 40.0f;

WandOfTeleportation::WandOfTeleportation()
{
	name = "Wand of Teleportation";
}

void WandOfTeleportation::onZap(int cell)
{
	Char* ch = Actor::findChar(cell);

	if (ch == curUser) 
	{

		setKnown();
		//ScrollOfTeleportation.teleportHero(curUser);

	}
	else if (ch != NULL) 
	{

		int count = 10;
		int pos;
		do {
			pos = Dungeon::level->randomRespawnCell();
			if (count-- <= 0) {
				break;
			}
		} while (pos == -1);

		if (pos == -1) {

			//GLog.w(ScrollOfTeleportation.TXT_NO_TELEPORT);

		}
		else {

			ch->pos = pos;
			ch->sprite->place(ch->pos);
			ch->sprite->visible = Dungeon::visible[pos];
			//GLog.i(curUser.name + " teleported " + ch.name + " to somewhere");

		}

	}
	else {

		//GLog.i("nothing happened");

	}
}

void WandOfTeleportation::fx(int cell, Callback* callback)
{
	//MagicMissile.coldLight(curUser.sprite.parent, curUser.pos, cell, callback);
	//Sample.INSTANCE.play(Assets.SND_ZAP);
}

WandOfSlowness::WandOfSlowness()
{
	name = "Wand of Slowness";
}

void WandOfSlowness::onZap(int cell)
{
	Char* ch = Actor::findChar(cell);
	if (ch != NULL) {

		//Buff.affect(ch, Slow.class, Slow.duration(ch) / 3 + power());

	}
	else {

		//GLog.i("nothing happened");

	}
}

void WandOfSlowness::fx(int cell, Callback* callback)
{
	//MagicMissile.slowness(curUser.sprite.parent, curUser.pos, cell, callback);
	//Sample.INSTANCE.play(Assets.SND_ZAP);
}

WandOfFirebolt::WandOfFirebolt()
{
	name = "Wand of Firebolt";
}

void WandOfFirebolt::onZap(int cell)
{
	int level = power();

	//for (int i = 1; i < Ballistica.distance - 1; i++) {
	//	int c = Ballistica.trace[i];
	//	if (Level.flamable[c]) {
	//		GameScene.add(Blob.seed(c, 1, Fire.class));
	//	}
	//}
	//
	//GameScene.add(Blob.seed(cell, 1, Fire.class));
	//
	//Char ch = Actor.findChar(cell);
	//if (ch != null) {
	//
	//	ch.damage(Random.Int(1, 8 + level * level), this);
	//	Buff.affect(ch, Burning.class).reignite(ch);
	//
	//	ch.sprite.emitter().burst(FlameParticle.FACTORY, 5);
	//
	//	if (ch == curUser && !ch.isAlive()) {
	//		Dungeon.fail(Utils.format(ResultDescriptions.WAND, name, Dungeon.depth));
	//		GLog.n("You killed yourself with your own Wand of Firebolt...");
	//	}
	//}
}

void WandOfFirebolt::fx(int cell, Callback* callback)
{
	//MagicMissile.fire(curUser.sprite.parent, curUser.pos, cell, callback);
	//Sample.INSTANCE.play(Assets.SND_ZAP);
}

WandOfPoison::WandOfPoison()
{
	name = "Wand of Poison";
}

void WandOfPoison::onZap(int cell)
{
	Char* ch = Actor::findChar(cell);
	if (ch != NULL) {

		//Buff.affect(ch, Poison.class).set(Poison.durationFactor(ch) * (5 + power()));

	}
	else {

		//GLog.i("nothing happened");

	}
}

void WandOfPoison::fx(int cell, Callback* callback)
{
	//MagicMissile.poison(curUser.sprite.parent, curUser.pos, cell, callback);
	//Sample.INSTANCE.play(Assets.SND_ZAP);
}

WandOfRegrowth::WandOfRegrowth()
{
	name = "Wand of Regrowth";
}

void WandOfRegrowth::onZap(int cell)
{
	//for (int i = 1; i < Ballistica.distance - 1; i++) {
	//	int p = Ballistica.trace[i];
	//	int c = Dungeon.level.map[p];
	//	if (c == Terrain.EMPTY ||
	//		c == Terrain.EMBERS ||
	//		c == Terrain.EMPTY_DECO) {
	//
	//		Level.set(p, Terrain.GRASS);
	//		GameScene.updateMap(p);
	//		if (Dungeon.visible[p]) {
	//			GameScene.discoverTile(p, c);
	//		}
	//
	//	}
	//}
	//
	//int c = Dungeon.level.map[cell];
	//if (c == Terrain.EMPTY ||
	//	c == Terrain.EMBERS ||
	//	c == Terrain.EMPTY_DECO ||
	//	c == Terrain.GRASS ||
	//	c == Terrain.HIGH_GRASS) {
	//
	//	GameScene.add(Blob.seed(cell, (power() + 2) * 20, Regrowth.class));
	//
	//}
	//else {
	//
	//	GLog.i("nothing happened");
	//
	//}
}

void WandOfRegrowth::fx(int cell, Callback* callback)
{
	//MagicMissile.foliage(curUser.sprite.parent, curUser.pos, cell, callback);
	//Sample.INSTANCE.play(Assets.SND_ZAP);
}

WandOfBlink::WandOfBlink()
{
	name = "Wand of Blink";
}

void WandOfBlink::appear(Char* ch, int pos)
{
	ch->sprite->interruptMotion();

	ch->move(pos);
	ch->sprite->place(pos);

	if (ch->invisible == 0) {
		ch->sprite->alpha(0);
		ch->sprite->parent->add(new AlphaTweener(ch->sprite, 1, 0.4f));
	}

	ch->sprite->emitter()->start(Speck::factory(Speck::LIGHT), 0.2f, 3);
	//Sample.INSTANCE.play(Assets.SND_TELEPORT);
}

void WandOfBlink::onZap(int cell)
{
	//int level = power();
	//
	//if (Ballistica.distance > level + 4) {
	//	cell = Ballistica.trace[level + 3];
	//}
	//else if (Actor.findChar(cell) != null && Ballistica.distance > 1) {
	//	cell = Ballistica.trace[Ballistica.distance - 2];
	//}
	//
	//curUser.sprite.visible = true;
	//appear(Dungeon.hero, cell);
	//Dungeon.observe();
}

void WandOfBlink::fx(int cell, Callback* callback)
{
	//MagicMissile.whiteLight(curUser.sprite.parent, curUser.pos, cell, callback);
	//Sample.INSTANCE.play(Assets.SND_ZAP);
	curUser->sprite->visible = false;
}

WandOfLightning::WandOfLightning()
{
	name = "Wand of Lightning";

	points.resize(20);
}

void WandOfLightning::hit(Char* ch, int damage)
{
	if (damage < 1) {
		return;
	}

	if (ch == Dungeon::hero) {
		Camera::mainCamera->shake(2, 0.3f);
	}

	affected.push_back(ch);
	//ch.damage(Level.water[ch.pos] && !ch.flying ? (int)(damage * 2) : damage, LightningTrap.LIGHTNING);

	//ch->sprite->centerEmitter()->burst(SparkParticle::FACTORY, 3);
	ch->sprite->flash();

	points[nPoints++] = ch->pos;

	std::set<Char*> ns;
	for (int i = 0; i < 8; i++) {
		Char* n = Actor::findChar(ch->pos + Level::NEIGHBOURS8[i]);
		if (n != NULL && std::find(affected.begin(), affected.end(), n) == affected.end()) {
			ns.insert(n);
		}
	}

	if (ns.size() > 0) {
		hit(RandomT<Char*>::element(ns), Random::Int(damage / 2, damage));
	}
}

void WandOfLightning::onZap(int cell)
{
	// Everything is processed in fx() method
	if (!curUser->isAlive()) 
	{
		//Dungeon.fail(Utils.format(ResultDescriptions.WAND, name, Dungeon.depth));
		GLog::n("You killed yourself with your own Wand of Lightning...");
	}
}

void WandOfLightning::fx(int cell, Callback* callback)
{
	nPoints = 0;
	points[nPoints++] = Dungeon::hero->pos;

	Char* ch = Actor::findChar(cell);
	if (ch != NULL) {

		affected.clear();
		int lvl = power();
		hit(ch, Random::Int(5 + lvl / 2, 10 + lvl));

	}
	else {

		points[nPoints++] = cell;
		//CellEmitter.center(cell).burst(SparkParticle.FACTORY, 3);

	}
	//curUser.sprite.parent.add(new Lightning(points, nPoints, callback));
}

WandOfAmok::WandOfAmok()
{
	name = "Wand of Amok";
}

void WandOfAmok::onZap(int cell)
{
	Char* ch = Actor::findChar(cell);
	if (ch != NULL) {

		if (ch == Dungeon::hero) {
			//Buff.affect(ch, Vertigo.class, Vertigo.duration(ch));
		}
		else {
			//Buff.affect(ch, Amok.class, 3f + power());
		}

	}
	else {

		GLog::i("nothing happened");

	}
}

void WandOfAmok::fx(int cell, Callback* callback)
{
	//MagicMissile.purpleLight(curUser.sprite.parent, curUser.pos, cell, callback);
	//Sample.INSTANCE.play(Assets.SND_ZAP);
}

WandOfReach::WandOfReach()
{
	name = "Wand of Reach";
	hitChars = false;
}

const std::string WandOfReach::TXT_YOU_NOW_HAVE = "You have magically transported %s into your backpack";

void WandOfReach::transport(Heap* heap)
{
	//Item item = heap.pickUp();
	//if (item.doPickUp(curUser)) {
	//
	//	if (item instanceof Dewdrop) {
	//		// Do nothing
	//	}
	//	else {
	//		if (((item instanceof ScrollOfUpgrade || item instanceof ScrollOfEnchantment) && ((Scroll)item).isKnown()) ||
	//			((item instanceof PotionOfStrength || item instanceof PotionOfMight) && ((Potion)item).isKnown())) {
	//			GLog.p(TXT_YOU_NOW_HAVE, item.name());
	//		}
	//		else {
	//			GLog.i(TXT_YOU_NOW_HAVE, item.name());
	//		}
	//	}
	//
	//}
	//else {
	//	Dungeon.level.drop(item, curUser.pos).sprite.drop();
	//}
}

void WandOfReach::onZap(int cell)
{
	//int reach = Math.min(Ballistica.distance, power() + 4);
	//
	//boolean mapUpdated = false;
	//for (int i = 1; i < reach; i++) {
	//
	//	int c = Ballistica.trace[i];
	//
	//	int before = Dungeon.level.map[c];
	//
	//	Char ch = Actor.findChar(c);
	//	if (ch != null) {
	//		Actor.addDelayed(new Swap(curUser, ch), -1);
	//		break;
	//	}
	//
	//	Heap heap = Dungeon.level.heaps.get(c);
	//	if (heap != null) {
	//		switch (heap.type) {
	//		case HEAP:
	//			transport(heap);
	//			break;
	//		case CHEST:
	//		case MIMIC:
	//		case TOMB:
	//		case SKELETON:
	//			heap.open(curUser);
	//			break;
	//		default:
	//		}
	//
	//		break;
	//	}
	//
	//	Dungeon.level.press(c, null);
	//	if (before == Terrain.OPEN_DOOR) {
	//		Level.set(c, Terrain.DOOR);
	//		GameScene.updateMap(c);
	//	}
	//	else if (Level.water[c]) {
	//		GameScene.ripple(c);
	//	}
	//
	//	mapUpdated = mapUpdated || Dungeon.level.map[c] != before;
	//}
	//
	//if (mapUpdated) {
	//	Dungeon.observe();
	//}
}

void WandOfReach::fx(int cell, Callback* callback)
{
	//MagicMissile.force(curUser.sprite.parent, curUser.pos, cell, callback);
	//Sample.INSTANCE.play(Assets.SND_ZAP);
}

WandOfFlock::WandOfFlock()
{
	name = "Wand of Flock";
}

void WandOfFlock::onZap(int cell)
{
//	int level = power();
//
//	int n = level + 2;
//
//	if (Actor.findChar(cell) != null && Ballistica.distance > 2) {
//		cell = Ballistica.trace[Ballistica.distance - 2];
//	}
//
//	boolean[] passable = BArray.or(Level.passable, Level.avoid, null);
//	for (Actor actor : Actor.all()) {
//		if (actor instanceof Char) {
//			passable[((Char)actor).pos] = false;
//		}
//	}
//
//	PathFinder.buildDistanceMap(cell, passable, n);
//	int dist = 0;
//
//	if (Actor.findChar(cell) != null) {
//		PathFinder.distance[cell] = Integer.MAX_VALUE;
//		dist = 1;
//	}
//
//	float lifespan = level + 3;
//
//sheepLabel:
//	for (int i = 0; i < n; i++) {
//		do {
//			for (int j = 0; j < Level.LENGTH; j++) {
//				if (PathFinder.distance[j] == dist) {
//
//					Sheep sheep = new Sheep();
//					sheep.lifespan = lifespan;
//					sheep.pos = j;
//					GameScene.add(sheep);
//					Dungeon.level.mobPress(sheep);
//
//					CellEmitter.get(j).burst(Speck.factory(Speck.WOOL), 4);
//
//					PathFinder.distance[j] = Integer.MAX_VALUE;
//
//					continue sheepLabel;
//				}
//			}
//			dist++;
//		} while (dist < n);
//	}
}

void WandOfFlock::fx(int cell, Callback* callback)
{
	//MagicMissile.wool(curUser.sprite.parent, curUser.pos, cell, callback);
	//Sample.INSTANCE.play(Assets.SND_ZAP);
}

WandOfFlock::Sheep::Sheep()
{
	name = "sheep";
	//spriteClass = SheepSprite.class;
	initialized = false;
}

CharSprite* WandOfFlock::Sheep::Sprite()
{
	return new SheepSprite();
}

void WandOfFlock::Sheep::interact()
{
	yell(RandomT<std::string>::element(
		std::list<std::string>(
			QUOTES, QUOTES+4)));
}

bool WandOfFlock::Sheep::act()
{
	if (initialized) 
	{
		HP = 0;

		destroy();
		sprite->Die();

	}
	else {
		initialized = true;
		spend(lifespan + Random::Float(2));
	}
	return true;
}

const std::string WandOfFlock::Sheep::QUOTES[4] = { "Baa!", "Baa?", "Baa.", "Baa..." };

WandOfDisintegration::WandOfDisintegration()
{
	name = "Wand of Disintegration";
	hitChars = false;
}

void WandOfDisintegration::onZap(int cell)
{
	//bool terrainAffected = false;
	//
	//int level = power();
	//
	//int maxDistance = Distance();
	//Ballistica.distance = Math.min(Ballistica.distance, maxDistance);
	//
	//ArrayList<Char> chars = new ArrayList<Char>();
	//
	//for (int i = 1; i < Ballistica.distance; i++) {
	//
	//	int c = Ballistica.trace[i];
	//
	//	Char ch;
	//	if ((ch = Actor.findChar(c)) != null) {
	//		chars.add(ch);
	//	}
	//
	//	int terr = Dungeon.level.map[c];
	//	if (terr == Terrain.DOOR || terr == Terrain.SIGN) {
	//
	//		Dungeon.level.destroy(c);
	//		GameScene.updateMap(c);
	//		terrainAffected = true;
	//
	//	}
	//	else if (terr == Terrain.HIGH_GRASS) {
	//
	//		Level.set(c, Terrain.GRASS);
	//		GameScene.updateMap(c);
	//		terrainAffected = true;
	//
	//	}
	//
	//	CellEmitter.center(c).burst(PurpleParticle.BURST, Random.IntRange(1, 2));
	//}
	//
	//if (terrainAffected) {
	//	Dungeon.observe();
	//}
	//
	//int lvl = level + chars.size();
	//int dmgMin = lvl;
	//int dmgMax = 8 + lvl * lvl / 3;
	//for (Char ch : chars) {
	//	ch.damage(Random.NormalIntRange(dmgMin, dmgMax), this);
	//	ch.sprite.centerEmitter().burst(PurpleParticle.BURST, Random.IntRange(1, 2));
	//	ch.sprite.flash();
	//}
}

void WandOfDisintegration::fx(int cell, Callback* callback)
{
	//cell = Ballistica.trace[Math.min(Ballistica.distance, distance()) - 1];
	//curUser.sprite.parent.add(new DeathRay(curUser.sprite.center(), DungeonTilemap.tileCenterToWorld(cell)));
	//callback.call();
}

WandOfAvalanche::WandOfAvalanche()
{
	name = "Wand of Avalanche";
	hitChars = false;
}

void WandOfAvalanche::onZap(int cell)
{
	//Sample.INSTANCE.play(Assets.SND_ROCKS);
	//
	//int level = power();
	//
	//Ballistica.distance = Math.min(Ballistica.distance, 8 + level);
	//
	//int size = 1 + level / 3;
	//PathFinder.buildDistanceMap(cell, BArray.not(Level.solid, null), size);
	//
	//int shake = 0;
	//for (int i = 0; i < Level.LENGTH; i++) {
	//
	//	int d = PathFinder.distance[i];
	//
	//	if (d < Integer.MAX_VALUE) {
	//
	//		Char ch = Actor.findChar(i);
	//		if (ch != null) {
	//
	//			ch.sprite.flash();
	//			ch.damage(Random.Int(2, 6 + (size - d) * 2), this);
	//
	//			if (ch.isAlive() && Random.Int(2 + d) == 0) {
	//				Buff.prolong(ch, Paralysis.class, Random.IntRange(2, 6));
	//			}
	//		}
	//
	//		if (ch != null && ch.isAlive()) {
	//			if (ch instanceof Mob) {
	//				Dungeon.level.mobPress((Mob)ch);
	//			}
	//			else {
	//				Dungeon.level.press(i, ch);
	//			}
	//		}
	//		else {
	//			Dungeon.level.press(i, null);
	//		}
	//
	//		if (Dungeon.visible[i]) {
	//			CellEmitter.get(i).start(Speck.factory(Speck.ROCK), 0.07f, 3 + (size - d));
	//			if (Level.water[i]) {
	//				GameScene.ripple(i);
	//			}
	//			if (shake < size - d) {
	//				shake = size - d;
	//			}
	//		}
	//	}
	//
	//	Camera.main.shake(3, 0.07f * (3 + shake));
	//}
	//
	//if (!curUser.isAlive()) {
	//	Dungeon.fail(Utils.format(ResultDescriptions.WAND, name, Dungeon.depth));
	//	GLog.n("You killed yourself with your own Wand of Avalanche...");
	//}
}

void WandOfAvalanche::fx(int cell, Callback* callback)
{
	//MagicMissile.earth(curUser.sprite.parent, curUser.pos, cell, callback);
	//Sample.INSTANCE.play(Assets.SND_ZAP);
}

const String WandOfMagicMissile::AC_DISENCHANT = "DISENCHANT";

void WandOfMagicMissile::actions(Hero* hero, std::vector<std::string>& actions)
{
	Wand::actions(hero, actions);
	if (Level() > 0) {
		actions.push_back(AC_DISENCHANT);
	}
}

void WandOfMagicMissile::execute(Hero* hero, const std::string& action)
{
	if (action.compare(AC_DISENCHANT) == 0) {

		if (hero->belongings->weapon == this) {
			disenchantEquipped = true;
			hero->belongings->weapon = NULL;
			updateQuickslot();
		}
		else {
			disenchantEquipped = false;
			detach(hero->belongings->backpack);
		}

		curUser = hero;
		GameScene::selectItem(itemSelector, WndBag::Mode::WAND, TXT_SELECT_WAND);

	}
	else {

		Wand::execute(hero, action);

	}
}

const String WandOfMagicMissile::TXT_SELECT_WAND = "Select a wand to upgrade";
const String WandOfMagicMissile::TXT_DISENCHANTED =
"you disenchanted the Wand of Magic Missile and used its essence to upgrade your %s";

const float WandOfMagicMissile::TIME_TO_DISENCHANT = 2.0f;

void WandOfMagicMissile::onZap(int cell)
{
	Char* ch = Actor::findChar(cell);
	if (ch != NULL) {

		int level = power();

		ch->damage(Random::Int(1, 6 + level * 2), this->getClassName());
		ch->sprite->burst(0xFF99CCFF, level / 2 + 2);

		if (ch == curUser && !ch->isAlive()) {
			Dungeon::fail(GameMath::format(ResultDescriptions::WAND.c_str(), name, Dungeon::depth));
			GLog::n("You killed yourself with your own Wand of Magic Missile...");
		}
	}
}

namespace{
	class WndBagListenerNew :public WndBag::Listener{
	public:
		WandOfMagicMissile* wand;
		WndBagListenerNew(WandOfMagicMissile* w) :wand(w){}

		virtual void onSelect(Item* item){
			if (item != NULL) {

				//Sample.INSTANCE.play(Assets.SND_EVOKE);
				ScrollOfUpgrade::upgrade(Item::curUser);
				Item::evoke(Item::curUser);

				GLog::w(WandOfMagicMissile::TXT_DISENCHANTED.c_str(), item->Name());

				item->upgrade();
				Item::curUser->spendAndNext(WandOfMagicMissile::TIME_TO_DISENCHANT);

				//Badges::validateItemLevelAquired(item);

			}
			else {
				if (wand->disenchantEquipped) {
					Item::curUser->belongings->weapon = wand;
					wand->updateQuickslot();
				}
				else {
					wand->collect(Item::curUser->belongings->backpack);
				}
			}
		}
	};
}
WandOfMagicMissile::WandOfMagicMissile()
{
	name = "Wand of Magic Missile";
	image = ItemSpriteSheet::WAND_MAGIC_MISSILE;

	itemSelector = new WndBagListenerNew(this);
}
