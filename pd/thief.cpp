#include "thief.h"
#include "mobsprite.h"
#include "util.h"

const std::string Thief::TXT_STOLE = "%s stole %s from you!";
const std::string Thief::TXT_CARRIES = "\n\n%s is carrying a _%s_. Stolen obviously.";

const std::string Thief::ITEM = "item";

Thief::Thief()
{
	name = "crazy thief";
	//spriteClass = ThiefSprite.class;

	HP = HT = 20;
	_defenseSkill = 12;

	EXP = 5;
	maxLvl = 10;

	//loot = RingOfHaggler.class;
	lootChance = 0.01f;

	//FLEEING = new Fleeing();

	item = NULL;
}

CharSprite* Thief::Sprite()
{
	return new ThiefSprite();
}

Mob* Thief::CreateThief()
{
	return new Thief();
}

int Thief::damageRoll()
{
	return Random::NormalIntRange(1, 7);
}

std::string Thief::description()
{
	std::string desc =
		std::string("Deeper levels of the dungeon have always been a hiding place for all kinds of criminals. ") +
		std::string("Not all of them could keep a clear mind during their extended periods so far from daylight. Long ago, ") +
		std::string("these crazy thieves and bandits have forgotten who they are and why they steal.");

	//if (item != null) {
	//	desc += String.format(TXT_CARRIES, Utils.capitalize(this.name), item.name());
	//}

	return desc;
}

void Thief::Fleeing::nowhereToRun()
{
	//if (buff(Terror.class) == null) {
	//	sprite.showStatus(CharSprite.NEGATIVE, TXT_RAGE);
	//	state = HUNTING;
	//}
	//else {
	//	super.nowhereToRun();
	//}
}
