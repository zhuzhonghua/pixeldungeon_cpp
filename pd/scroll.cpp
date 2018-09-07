#include "scroll.h"
#include "simpleresource.h"
#include "cellemitter.h"
#include "speck.h"
#include "hero.h"

const String Scroll::TXT_BLINDED = "You can't read a scroll while blinded";

String Scroll::_scrolls[] = {
	"ScrollOfIdentify",
	"ScrollOfMagicMapping",
	"ScrollOfRecharging",
	"ScrollOfRemoveCurse",
	"ScrollOfTeleportation",
	"ScrollOfChallenge",
	"ScrollOfTerror",
	"ScrollOfLullaby",
	"ScrollOfPsionicBlast",
	"ScrollOfMirrorImage",
	"ScrollOfUpgrade",
	"ScrollOfEnchantment"
};

std::vector<String> Scroll::scrolls(_scrolls, _scrolls+sizeof(_scrolls)/sizeof(String));

const String Scroll::AC_READ = "READ";


const float Scroll::TIME_TO_READ = 1.0f;

String Scroll::_runes[] =
{ "KAUNAN", "SOWILO", "LAGUZ", "YNGVI", "GYFU", "RAIDO", "ISAZ", "MANNAZ", "NAUDIZ", "BERKANAN", "ODAL", "TIWAZ" };

std::vector<String> Scroll::runes(_runes, _runes + sizeof(_runes) / sizeof(String));

int Scroll::_images[] = {
		ItemSpriteSheet::SCROLL_KAUNAN,
		ItemSpriteSheet::SCROLL_SOWILO,
		ItemSpriteSheet::SCROLL_LAGUZ,
		ItemSpriteSheet::SCROLL_YNGVI,
		ItemSpriteSheet::SCROLL_GYFU,
		ItemSpriteSheet::SCROLL_RAIDO,
		ItemSpriteSheet::SCROLL_ISAZ,
		ItemSpriteSheet::SCROLL_MANNAZ,
		ItemSpriteSheet::SCROLL_NAUDIZ,
		ItemSpriteSheet::SCROLL_BERKANAN,
		ItemSpriteSheet::SCROLL_ODAL,
		ItemSpriteSheet::SCROLL_TIWAZ};

std::vector<int> Scroll::images(_images, _images+sizeof(_images)/sizeof(int));

ItemStatusHandler* Scroll::handler;

Scroll::Scroll()
{
	stackable = true;
	defaultAction = AC_READ;

	//image = handler.image(this);
	//rune = handler.label(this);
}

void Scroll::initLabels()
{
	handler = new ItemStatusHandler(scrolls, runes, images);
}

void Scroll::save(Bundle* bundle)
{
	handler->save(bundle);
}

void Scroll::restore(Bundle* bundle)
{
	handler = new ItemStatusHandler(scrolls, runes, images, bundle);
}

void Scroll::actions(Hero* hero, std::vector<std::string>& actions)
{
	Item::actions(hero, actions);
	actions.push_back(AC_READ);
}

void Scroll::execute(Hero* hero, const std::string action)
{
	if (action.compare(AC_READ) == 0) {

		//if (hero.buff(Blindness.class) != null) {
		//	GLog.w(TXT_BLINDED);
		//}
		//else {
		//	curUser = hero;
		//	curItem = detach(hero.belongings.backpack);
		//	doRead();
		//}

	}
	else {

		Item::execute(hero, action);

	}
}


bool Scroll::isKnown()
{
	return true;// handler.isKnown(this);
}

void Scroll::setKnown()
{
	//if (!isKnown()) {
	//	handler.know(this);
	//}
	//
	//Badges.validateAllScrollsIdentified();
}

Item* Scroll::identify()
{
	setKnown();
	return Item::identify();
}

String Scroll::info()
{
	return isKnown() ?
		desc() :
		std::string("This parchment is covered with indecipherable writing, and bears a title ") +
		std::string("of rune ") + rune + std::string(". Who knows what it will do when read aloud?");
}

std::set<std::string> Scroll::getKnown()
{
	return handler->known;
}

void Scroll::readAnimation()
{
	//curUser->spend(TIME_TO_READ);
	//curUser->busy();
	//((HeroSprite)curUser.sprite).read();
}

const String InventoryScroll::TXT_WARNING = "Do you really want to cancel this scroll usage? It will be consumed anyway.";
const String InventoryScroll::TXT_YES = "Yes, I'm positive";
const String InventoryScroll::TXT_NO = "No, I changed my mind";

boolean InventoryScroll::identifiedByUse = false;

InventoryScroll::InventoryScroll()
{
	inventoryTitle = "Select an item";
}

void InventoryScroll::doRead()
{
	if (!isKnown()) 
	{
		setKnown();
		identifiedByUse = true;
	}
	else {
		identifiedByUse = false;
	}

	//GameScene.selectItem(itemSelector, mode, inventoryTitle);
}

void InventoryScroll::confirmCancelation()
{
	//GameScene.show(new WndOptions(name(), TXT_WARNING, TXT_YES, TXT_NO){
	//	@Override
	//	protected void onSelect(int index) {
	//		switch (index) {
	//		case 0:
	//			curUser.spendAndNext(TIME_TO_READ);
	//			identifiedByUse = false;
	//			break;
	//		case 1:
	//			GameScene.selectItem(itemSelector, mode, inventoryTitle);
	//			break;
	//		}
	//	}
	//	public void onBackPressed() {};
	//});
}

ScrollOfIdentify::ScrollOfIdentify()
{
	name = "Scroll of Identify";
	inventoryTitle = "Select an item to identify";
	//mode = WndBag.Mode.UNIDENTIFED;
}

void ScrollOfIdentify::onItemSelected(Item* item)
{
	//curUser.sprite.parent.add(new Identification(curUser.sprite.center().offset(0, -16)));
	//
	//item.identify();
	//GLog.i("It is " + item);
	//
	//Badges.validateItemLevelAquired(item);
}

ScrollOfMagicMapping::ScrollOfMagicMapping()
{
	name = "Scroll of Magic Mapping";
}

void ScrollOfMagicMapping::discover(int cell)
{
	CellEmitter::get(cell)->start(Speck::factory(Speck::DISCOVER), 0.1f, 4);
}

const String ScrollOfMagicMapping::TXT_LAYOUT = "You are now aware of the level layout.";

void ScrollOfMagicMapping::doRead()
{
	//int length = Level.LENGTH;
	//int[] map = Dungeon.level.map;
	//boolean[] mapped = Dungeon.level.mapped;
	//boolean[] discoverable = Level.discoverable;
	//
	//boolean noticed = false;
	//
	//for (int i = 0; i < length; i++) {
	//
	//	int terr = map[i];
	//
	//	if (discoverable[i]) {
	//
	//		mapped[i] = true;
	//		if ((Terrain.flags[terr] & Terrain.SECRET) != 0) {
	//
	//			Level.set(i, Terrain.discover(terr));
	//			GameScene.updateMap(i);
	//
	//			if (Dungeon.visible[i]) {
	//				GameScene.discoverTile(i, terr);
	//				discover(i);
	//
	//				noticed = true;
	//			}
	//		}
	//	}
	//}
	//Dungeon.observe();
	//
	//GLog.i(TXT_LAYOUT);
	//if (noticed) {
	//	Sample.INSTANCE.play(Assets.SND_SECRET);
	//}
	//
	//SpellSprite.show(curUser, SpellSprite.MAP);
	//Sample.INSTANCE.play(Assets.SND_READ);
	//Invisibility.dispel();
	//
	//setKnown();
	//
	//readAnimation();
}

ScrollOfRecharging::ScrollOfRecharging()
{
	name = "Scroll of Recharging";
}

void ScrollOfRecharging::charge(Hero* hero)
{
	//hero.sprite.centerEmitter().burst( EnergyParticle.FACTORY, 15 );
}

void ScrollOfRecharging::doRead()
{
	//int count = curUser.belongings.charge(true);
	//charge(curUser);
	//
	//Sample.INSTANCE.play(Assets.SND_READ);
	//Invisibility.dispel();
	//
	//if (count > 0) {
	//	GLog.i("a surge of energy courses through your pack, recharging your wand" + (count > 1 ? "s" : ""));
	//	SpellSprite.show(curUser, SpellSprite.CHARGE);
	//}
	//else {
	//	GLog.i("a surge of energy courses through your pack, but nothing happens");
	//}
	//setKnown();
	//
	//readAnimation();
}

ScrollOfRemoveCurse::ScrollOfRemoveCurse()
{
	name = "Scroll of Remove Curse";
}

boolean ScrollOfRemoveCurse::uncurse(Hero* hero, const std::vector<Item*>& items)
{
	boolean procced = false;
	for (int i = 0; i < items.size(); i++) {
		Item* item = items[i];
		if (item != NULL && item->cursed) {
			item->cursed = false;
			procced = true;
		}
	}

	//if (procced) {
	//	hero.sprite.emitter().start(ShadowParticle.UP, 0.05f, 10);
	//}

	return procced;
}

void ScrollOfRemoveCurse::doRead()
{
	//new Flare(6, 32).show(curUser.sprite, 2f);
	//Sample.INSTANCE.play(Assets.SND_READ);
	//Invisibility.dispel();
	//
	//boolean procced = uncurse(curUser, curUser.belongings.backpack.items.toArray(new Item[0]));
	//procced = uncurse(curUser,
	//	curUser.belongings.weapon,
	//	curUser.belongings.armor,
	//	curUser.belongings.ring1,
	//	curUser.belongings.ring2) || procced;
	//
	//Weakness.detach(curUser, Weakness.class);
	//
	//if (procced) {
	//	GLog.p(TXT_PROCCED);
	//}
	//else {
	//	GLog.i(TXT_NOT_PROCCED);
	//}
	//
	//setKnown();
	//
	//readAnimation();
}

const String ScrollOfRemoveCurse::TXT_PROCCED =
"Your pack glows with a cleansing light, and a malevolent energy disperses.";
const String ScrollOfRemoveCurse::TXT_NOT_PROCCED =
"Your pack glows with a cleansing light, but nothing happens.";

const String ScrollOfTeleportation::TXT_TELEPORTED =
"In a blink of an eye you were teleported to another location of the level.";

const String ScrollOfTeleportation::TXT_NO_TELEPORT =
"Strong magic aura of this place prevents you from teleporting!";

ScrollOfTeleportation::ScrollOfTeleportation()
{
	name = "Scroll of Teleportation";
}

void ScrollOfTeleportation::teleportHero(Hero* hero)
{
	//int count = 10;
	//int pos;
	//do {
	//	pos = Dungeon.level.randomRespawnCell();
	//	if (count-- <= 0) {
	//		break;
	//	}
	//} while (pos == -1);
	//
	//if (pos == -1) {
	//
	//	GLog.w(TXT_NO_TELEPORT);
	//
	//}
	//else {
	//
	//	WandOfBlink.appear(hero, pos);
	//	Dungeon.level.press(pos, hero);
	//	Dungeon.observe();
	//
	//	GLog.i(TXT_TELEPORTED);
	//
	//}
}

void ScrollOfTeleportation::doRead()
{	//
	//Sample.INSTANCE.play(Assets.SND_READ);
	//Invisibility.dispel();
	//
	//teleportHero(curUser);
	//setKnown();
	//
	//readAnimation();
}

ScrollOfChallenge::ScrollOfChallenge()
{
	name = "Scroll of Challenge";
}

void ScrollOfChallenge::doRead()
{
	//for (Mob mob : Dungeon.level.mobs.toArray(new Mob[0])) {
	//	mob.beckon(curUser.pos);
	//	if (Dungeon.visible[mob.pos]) {
	//		Buff.affect(mob, Rage.class, Level.distance(curUser.pos, mob.pos));
	//	}
	//}
	//
	//for (Heap heap : Dungeon.level.heaps.values()) {
	//	if (heap.type == Heap.Type.MIMIC) {
	//		Mimic m = Mimic.spawnAt(heap.pos, heap.items);
	//		if (m != null) {
	//			m.beckon(curUser.pos);
	//			heap.destroy();
	//		}
	//	}
	//}
	//
	//GLog.w("The scroll emits a challenging roar that echoes throughout the dungeon!");
	//setKnown();
	//
	//curUser.sprite.centerEmitter().start(Speck.factory(Speck.SCREAM), 0.3f, 3);
	//Sample.INSTANCE.play(Assets.SND_CHALLENGE);
	//Invisibility.dispel();
	//
	//readAnimation();
}

ScrollOfTerror::ScrollOfTerror()
{
	name = "Scroll of Terror";
}

void ScrollOfTerror::doRead()
{
	//new Flare(5, 32).color(0xFF0000, true).show(curUser.sprite, 2f);
	//Sample.INSTANCE.play(Assets.SND_READ);
	//Invisibility.dispel();
	//
	//int count = 0;
	//Mob affected = null;
	//for (Mob mob : Dungeon.level.mobs.toArray(new Mob[0])) {
	//	if (Level.fieldOfView[mob.pos]) {
	//		Buff.affect(mob, Terror.class, Terror.DURATION).object = curUser.id();
	//
	//		count++;
	//		affected = mob;
	//	}
	//}
	//
	//switch (count) {
	//case 0:
	//	GLog.i("The scroll emits a brilliant flash of red light");
	//	break;
	//case 1:
	//	GLog.i("The scroll emits a brilliant flash of red light and the " + affected.name + " flees!");
	//	break;
	//default:
	//	GLog.i("The scroll emits a brilliant flash of red light and the monsters flee!");
	//}
	//setKnown();
	//
	//readAnimation();
}

ScrollOfLullaby::ScrollOfLullaby()
{
	name = "Scroll of Lullaby";
}

void ScrollOfLullaby::doRead()
{
	//curUser.sprite.centerEmitter().start(Speck.factory(Speck.NOTE), 0.3f, 5);
	//Sample.INSTANCE.play(Assets.SND_LULLABY);
	//Invisibility.dispel();
	//
	//int count = 0;
	//Mob affected = null;
	//for (Mob mob : Dungeon.level.mobs.toArray(new Mob[0])) {
	//	if (Level.fieldOfView[mob.pos]) {
	//		Buff.affect(mob, Sleep.class);
	//		if (mob.buff(Sleep.class) != null) {
	//			affected = mob;
	//			count++;
	//		}
	//	}
	//}
	//
	//switch (count) {
	//case 0:
	//	GLog.i("The scroll utters a soothing melody.");
	//	break;
	//case 1:
	//	GLog.i("The scroll utters a soothing melody and the " + affected.name + " falls asleep!");
	//	break;
	//default:
	//	GLog.i("The scroll utters a soothing melody and the monsters fall asleep!");
	//}
	//setKnown();
	//
	//readAnimation();
}

ScrollOfPsionicBlast::ScrollOfPsionicBlast()
{
	name = "Scroll of Psionic Blast";
}

void ScrollOfPsionicBlast::doRead()
{
	//GameScene.flash(0xFFFFFF);
	//
	//Sample.INSTANCE.play(Assets.SND_BLAST);
	//Invisibility.dispel();
	//
	//for (Mob mob : Dungeon.level.mobs.toArray(new Mob[0])) {
	//	if (Level.fieldOfView[mob.pos]) {
	//		Buff.prolong(mob, Blindness.class, Random.Int(3, 6));
	//		mob.damage(Random.IntRange(1, mob.HT * 2 / 3), this);
	//	}
	//}
	//
	//Buff.prolong(curUser, Blindness.class, Random.Int(3, 6));
	//Dungeon.observe();
	//
	//setKnown();
	//
	//readAnimation();
}

ScrollOfMirrorImage::ScrollOfMirrorImage()
{
	name = "Scroll of Mirror Image";
}

void ScrollOfMirrorImage::doRead()
{
	//ArrayList<Integer> respawnPoints = new ArrayList<Integer>();
	//
	//for (int i = 0; i < Level.NEIGHBOURS8.length; i++) {
	//	int p = curUser.pos + Level.NEIGHBOURS8[i];
	//	if (Actor.findChar(p) == null && (Level.passable[p] || Level.avoid[p])) {
	//		respawnPoints.add(p);
	//	}
	//}
	//
	//int nImages = NIMAGES;
	//while (nImages > 0 && respawnPoints.size() > 0) {
	//	int index = Random.index(respawnPoints);
	//
	//	MirrorImage mob = new MirrorImage();
	//	mob.duplicate(curUser);
	//	GameScene.add(mob);
	//	WandOfBlink.appear(mob, respawnPoints.get(index));
	//
	//	respawnPoints.remove(index);
	//	nImages--;
	//}
	//
	//if (nImages < NIMAGES) {
	//	setKnown();
	//}
	//
	//Sample.INSTANCE.play(Assets.SND_READ);
	//Invisibility.dispel();
	//
	//readAnimation();
}

const String ScrollOfUpgrade::TXT_LOOKS_BETTER = "your %s certainly looks better now";

ScrollOfUpgrade::ScrollOfUpgrade()
{
	name = "Scroll of Upgrade";
	inventoryTitle = "Select an item to upgrade";
	//mode = WndBag.Mode.UPGRADEABLE;
}

void ScrollOfUpgrade::upgrade(Hero* hero)
{
	//hero->sprite->emitter()->start(Speck::factory(Speck::UP), 0.2f, 3);
}

void ScrollOfUpgrade::onItemSelected(Item* item)
{
	//ScrollOfRemoveCurse.uncurse(Dungeon.hero, item);
	//if (item.isBroken()) {
	//	item.fix();
	//}
	//else {
	//	item.upgrade();
	//}
	//
	//upgrade(curUser);
	//GLog.p(TXT_LOOKS_BETTER, item.name());
	//
	//Badges.validateItemLevelAquired(item);
}

const String ScrollOfEnchantment::TXT_GLOWS = "your %s glows in the dark";

void ScrollOfEnchantment::onItemSelected(Item* item)
{
	//ScrollOfRemoveCurse.uncurse(Dungeon.hero, item);
	//
	//if (item instanceof Weapon) {
	//
	//	((Weapon)item).enchant();
	//
	//}
	//else {
	//
	//	((Armor)item).inscribe();
	//
	//}
	//
	//item.fix();
	//
	//curUser.sprite.emitter().start(Speck.factory(Speck.LIGHT), 0.1f, 5);
	//Enchanting.show(curUser, item);
	//GLog.w(TXT_GLOWS, item.name());
}

ScrollOfEnchantment::ScrollOfEnchantment()
{
	name = "Scroll of Enchantment";
	inventoryTitle = "Select an enchantable item";
	//mode = WndBag.Mode.ENCHANTABLE;
}
