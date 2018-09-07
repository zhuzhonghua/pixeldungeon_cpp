#include "ring.h"
#include "simpleresource.h"
#include "util.h"
#include "glog.h"
#include "dungeon.h"
#include "hero.h"

const float Ring::TIME_TO_EQUIP = 1.0f;

const char* Ring::TXT_IDENTIFY = "you are now familiar enough with your %s to identify it. It is %s.";
const char* Ring::TXT_UNEQUIP_TITLE = "Unequip one ring";

const std::string Ring::TXT_UNEQUIP_MESSAGE =
std::string("You can only wear two rings at a time. ") +
std::string("Unequip one of your equipped rings.");

std::string Ring::_gems[] = { "diamond", "opal", "garnet", "ruby", "amethyst", "topaz", "onyx", "tourmaline", "emerald", "sapphire", "quartz", "agate" };
std::vector<std::string> Ring::gems(_gems, _gems + sizeof(_gems) / sizeof(std::string));

const std::string Ring::UNFAMILIRIARITY = "unfamiliarity";

int Ring::_images[] = {
	ItemSpriteSheet::RING_DIAMOND,
	ItemSpriteSheet::RING_OPAL,
	ItemSpriteSheet::RING_GARNET,
	ItemSpriteSheet::RING_RUBY,
	ItemSpriteSheet::RING_AMETHYST,
	ItemSpriteSheet::RING_TOPAZ,
	ItemSpriteSheet::RING_ONYX,
	ItemSpriteSheet::RING_TOURMALINE,
	ItemSpriteSheet::RING_EMERALD,
	ItemSpriteSheet::RING_SAPPHIRE,
	ItemSpriteSheet::RING_QUARTZ,
	ItemSpriteSheet::RING_AGATE };

std::vector<int> Ring::images(_images, _images + sizeof(_images) / sizeof(int));

std::string Ring::_rings[] = { 
	"RingOfMending",
	"RingOfDetection",
	"RingOfShadows",
	"RingOfPower",
	"RingOfHerbalism",
	"RingOfAccuracy",
	"RingOfEvasion",
	"RingOfSatiety",
	"RingOfHaste",
	"RingOfHaggler",
	"RingOfElements",
	"RingOfThorns" };

std::vector<std::string> Ring::rings(_rings, _rings+sizeof(_rings)/sizeof(std::string));

ItemStatusHandler* Ring::handler;

void Ring::renewBuff()
{
	if (buff != NULL) 
	{
		Char* owner = buff->target;
		buff->detach();
		if ((buff = Buff()) != NULL) 
		{
			buff->attachTo(owner);
		}
	}
}

void Ring::setKnown()
{
	//if (!isKnown()) {
	//	handler.know(this);
	//}
	//
	//Badges.validateAllRingsIdentified();
}

Ring::Ring()
{
	ticksToKnow = TICKS_TO_KNOW;

	syncGem();
}

void Ring::initGems()
{
	handler = new ItemStatusHandler(rings, gems, images);
}

void Ring::save(Bundle* bundle)
{
	//handler.save( bundle );
}

void Ring::restore(Bundle* bundle)
{
	handler = new ItemStatusHandler(rings, gems, images, bundle );
}

void Ring::syncGem()
{
	//image = handler.image(this);
	//gem = handler.label(this);
}

void Ring::actions(Hero* hero, std::vector<std::string>& actions)
{
	EquipableItem::actions(hero, actions);
	//actions.push_back(isEquipped(hero) ? AC_UNEQUIP : AC_EQUIP);
}

bool Ring::doEquip(Hero* hero)
{
	//if (hero.belongings.ring1 != null && hero.belongings.ring2 != null) {
	//
	//	final Ring r1 = hero.belongings.ring1;
	//	final Ring r2 = hero.belongings.ring2;
	//
	//	PixelDungeon.scene().add(
	//		new WndOptions(TXT_UNEQUIP_TITLE, TXT_UNEQUIP_MESSAGE,
	//		Utils.capitalize(r1.toString()),
	//		Utils.capitalize(r2.toString())){
	//
	//		@Override
	//		protected void onSelect(int index) {
	//
	//			detach(hero.belongings.backpack);
	//
	//			Ring equipped = (index == 0 ? r1 : r2);
	//			if (equipped.doUnequip(hero, true, false)) {
	//				doEquip(hero);
	//			}
	//			else {
	//				collect(hero.belongings.backpack);
	//			}
	//		}
	//	});
	//
	//	return false;
	//
	//}
	//else {
	//
	//	if (hero.belongings.ring1 == null) {
	//		hero.belongings.ring1 = this;
	//	}
	//	else {
	//		hero.belongings.ring2 = this;
	//	}
	//
	//	detach(hero.belongings.backpack);
	//
	//	activate(hero);
	//
	//	cursedKnown = true;
	//	if (cursed) {
	//		equipCursed(hero);
	//		GLog.n("your " + this + " tightens around your finger painfully");
	//	}
	//
	//	hero.spendAndNext(TIME_TO_EQUIP);
	//	return true;
	//
	//}
	return true;
}

void Ring::activate(Char* ch)
{
	//buff = buff();
	//buff.attachTo(ch);
}

bool Ring::doUnequip(Hero* hero, bool collect, bool single)
{
	//if (EquipableItem::doUnequip(hero, collect, single)) 
	//{
	//	if (hero.belongings.ring1 == this) {
	//		hero.belongings.ring1 = null;
	//	}
	//	else {
	//		hero.belongings.ring2 = null;
	//	}
	//
	//	hero.remove(buff);
	//	buff = null;
	//
	//	return true;
	//
	//}
	//else {

		return false;

	//}
}

bool Ring::isEquipped(Hero* hero)
{
	return true;// hero.belongings.ring1 == this || hero.belongings.ring2 == this;
}

int Ring::effectiveLevel()
{
	return 1;// isBroken() ? 1 : level();
}

void Ring::getBroken()
{
	renewBuff();
	EquipableItem::getBroken();
}

void Ring::fix()
{
	EquipableItem::fix();
	renewBuff();
}

int Ring::maxDurability(int lvl)
{
	if (lvl <= 1) 
	{
		return std::numeric_limits<int>::max();
	}
	else 
	{
		return 100 * (lvl < 16 ? 16 - lvl : 1);
	}
}

bool Ring::isKnown()
{
	return true;// handler.isKnown(this);
}

std::string Ring::toString()
{
	return "";
		//levelKnown && isBroken() ?
		//"broken " + super.toString() :
		//super.toString();
}

std::string Ring::Name()
{
	return "ring";// isKnown() ? name : gem + " ring";
}

std::string Ring::desc()
{
	return
		std::string("This metal band is adorned with a large ") + gem + std::string(" gem ") +
		std::string("that glitters in the darkness. Who knows what effect it has when worn?");
}

std::string Ring::info()
{
	//if (isEquipped(Dungeon.hero)) {
	//
	//	return desc() + "\n\n" + "The " + name() + " is on your finger" +
	//		(cursed ? ", and because it is cursed, you are powerless to remove it." : ".");
	//
	//}
	//else if (cursed && cursedKnown) {
	//
	//	return desc() + "\n\nYou can feel a malevolent magic lurking within the " + name() + ".";
	//
	//}
	//else {

		return desc();

	//}
}

bool Ring::isIdentified()
{
	return EquipableItem::isIdentified() && isKnown();
}

Item* Ring::identify()
{
	//setKnown();
	return EquipableItem::identify();
}

Item* Ring::random()
{
	int lvl = Random::Int(1, 3);
	if (Random::Float() < 0.3f) 
	{
		//degrade(lvl);
		cursed = true;
	}
	else 
	{
		//upgrade(lvl);
	}
	return this;
}

bool Ring::allKnown()
{
	return true;// handler.known().size() == rings.length - 2;
}

int Ring::price()
{
	return 80;// considerState(80);
}

void Ring::storeInBundle(Bundle* bundle)
{
	EquipableItem::storeInBundle(bundle);
	bundle->put(UNFAMILIRIARITY, ticksToKnow);
}

void Ring::restoreFromBundle(Bundle* bundle)
{
	EquipableItem::restoreFromBundle(bundle);
	if ((ticksToKnow = bundle->getInt(UNFAMILIRIARITY)) == 0) 
	{
		ticksToKnow = TICKS_TO_KNOW;
	}
}

const std::string Ring::RingBuff::TXT_KNOWN = "This is a %s";

Ring::RingBuff::RingBuff(Ring* r)
:ring(r)
{
	level = ring->effectiveLevel();
}

bool Ring::RingBuff::attachTo(Char* target)
{
	if (dynamic_cast<Hero*>(target) != NULL && ((Hero*)target)->heroClass.type() == HeroClass::E_ROGUE && !ring->isKnown()) 
	{
		ring->setKnown();
		GLog::i(TXT_KNOWN.c_str(), ring->Name().c_str());
		//Badges::validateItemLevelAquired(ring);
	}

	return Buff::attachTo(target);
}

bool Ring::RingBuff::act()
{
	if (!ring->isIdentified() && --ring->ticksToKnow <= 0) 
	{
		std::string gemName = ring->Name();
		ring->identify();
		GLog::w(TXT_IDENTIFY, gemName, ring->toString().c_str());
		//Badges::validateItemLevelAquired(ring);
	}

	ring->use();

	spend(TICK);

	return true;
}

RingOfMending::RingOfMending()
{
	name = "Ring of Mending";
}

std::string RingOfMending::desc()
{
	return isKnown() ?
		std::string("This ring increases the body's regenerative properties, allowing ") +
		std::string("one to recover lost health at an accelerated rate. Degraded rings will ") +
		std::string("decrease or even halt one's natural regeneration.") :
		Ring::desc();
}

RingOfDetection::RingOfDetection()
{
	name = "Ring of Detection";
}

bool RingOfDetection::doEquip(Hero* hero)
{
	if (Ring::doEquip(hero)) 
	{
		Dungeon::hero->search(false);
		return true;
	}
	else 
	{
		return false;
	}
}

RingOfShadows::RingOfShadows()
{
	name = "Ring of Shadows";
}

std::string RingOfShadows::desc()
{
	return isKnown() ?
		std::string("Enemies will be less likely to notice you if you wear this ring. Degraded rings ") +
		std::string("of shadows will alert enemies who might otherwise not have noticed your presence.") :
		Ring::desc();
}

std::string RingOfPower::desc()
{
	return isKnown() ?
		std::string("Your wands will become more powerful in the energy field ") +
		std::string("that radiates from this ring. Degraded rings of power will instead weaken your wands.") :
		Ring::desc();
}

std::string RingOfHerbalism::desc()
{
	return isKnown() ?
		std::string("This ring increases your chance to gather dew and seeds from trampled grass.") :
		Ring::desc();
}

std::string RingOfAccuracy::desc()
{
	return isKnown() ?
		std::string("This ring increases your chance to hit the enemy.") :
		Ring::desc();
}

std::string RingOfEvasion::desc()
{
	return isKnown() ?
		std::string("This ring increases your chance to dodge enemy attack.") :
		Ring::desc();
}

std::string RingOfSatiety::desc()
{
	return isKnown() ?
		std::string("Wearing this ring you can go without food longer. Degraded rings of satiety will cause the opposite effect.") :
		Ring::desc();
}

std::string RingOfHaste::desc()
{
	return isKnown() ?
		std::string("This ring accelerates the wearer's flow of time, allowing one to perform all actions a little faster.") :
		Ring::desc();
}

std::string RingOfHaggler::desc()
{
	return isKnown() ?
		std::string("In fact this ring doesn't provide any magic effect, but it demonstrates ") +
		std::string("to shopkeepers and vendors, that the owner of the ring is a member of ") +
		std::string("The Thieves' Guild. Usually they are glad to give a discount in exchange ") +
		std::string("for temporary immunity guarantee. Upgrading this ring won't give any additional ") +
		std::string("bonuses.") :
		Ring::desc();
}

bool RingOfHaggler::doPickUp(Hero* hero)
{
	identify();
	//Badges::validateRingOfHaggler();
	//Badges::validateItemLevelAquired(this);
	return Ring::doPickUp(hero);
}

//REFLECTBUFF2(RingOfHagglerHaggling, RingOfHaggler::Haggling);

RingOfElements::RingOfElements()
{
	name = "Ring of Elements";

	if (FULL.size() <= 0)
	{
		FULL.insert("Burning");
		FULL.insert("ToxicGas");
		FULL.insert("BuffPoison");
		FULL.insert("LightningTrap::Electricity");
		FULL.insert("Warlock");
		FULL.insert("Eye");
		FULL.insert("Yog::BurningFist");
	}
}

std::string RingOfElements::desc()
{
	return isKnown() ?
		std::string("This ring provides resistance to different elements, such as fire, ") +
		std::string("electricity, gases etc. Also it decreases duration of negative effects.") :
		Ring::desc();
}

std::set<std::string> RingOfElements::EMPTY;
std::set<std::string> RingOfElements::FULL;

std::set<std::string> RingOfElements::Resistance::resistances()
{
	if (Random::Int(level + 3) >= 3) 
	{
		return FULL;
	}
	else
	{
		return EMPTY;
	}
}

std::string RingOfThorns::desc()
{
	return isKnown() ?
		std::string("Though this ring doesn't provide real thorns, an enemy that attacks you ") +
		std::string("will itself be wounded by a fraction of the damage that it inflicts. ") +
		std::string("Upgrading this ring won't give any additional bonuses.") :
		Ring::desc();
}

bool RingOfThorns::doPickUp(Hero* hero)
{
	identify();
	//Badges.validateRingOfThorns();
	//Badges.validateItemLevelAquired(this);
	return Ring::doPickUp(hero);
}
