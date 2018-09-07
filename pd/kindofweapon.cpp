#include "kindofweapon.h"
#include "hero.h"
#include "belongings.h"
#include "util.h"

const std::string KindOfWeapon::TXT_EQUIP_CURSED = "you wince as your grip involuntarily tightens around your %s";

const float KindOfWeapon::TIME_TO_EQUIP = 1.0f;

void KindOfWeapon::actions(Hero* hero, std::vector<std::string>& actions)
{
	EquipableItem::actions(hero, actions);
	actions.push_back(isEquipped(hero) ? AC_UNEQUIP : AC_EQUIP);
}

bool KindOfWeapon::isEquipped(Hero* hero)
{
	return true;// hero.belongings.weapon == this;
}

bool KindOfWeapon::doEquip(Hero* hero)
{
	//detachAll(hero.belongings.backpack);
	//
	//if (hero.belongings.weapon == null || hero.belongings.weapon.doUnequip(hero, true)) {
	//
	//	hero.belongings.weapon = this;
	//	activate(hero);
	//
	//	QuickSlot.refresh();
	//
	//	cursedKnown = true;
	//	if (cursed) {
	//		equipCursed(hero);
	//		GLog.n(TXT_EQUIP_CURSED, name());
	//	}
	//
	//	hero.spendAndNext(TIME_TO_EQUIP);
	//	return true;
	//
	//}
	//else {
	//
	//	collect(hero.belongings.backpack);
	//	return false;
	//}
	return false;
}

bool KindOfWeapon::doUnequip(Hero* hero, bool collect, bool single)
{
	if (EquipableItem::doUnequip(hero, collect, single)) 
	{
		hero->belongings->weapon = NULL;
		return true;
	}
	else 
	{
		return false;
	}
}

int KindOfWeapon::damageRoll(Hero* owner)
{
	return Random::NormalIntRange(min(), max());
}
