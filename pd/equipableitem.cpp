#include "equipableitem.h"
#include "glog.h"
#include "hero.h"

const std::string EquipableItem::TXT_UNEQUIP_CURSED = "You can't remove cursed %s!";
const std::string EquipableItem::AC_EQUIP = "EQUIP";
const std::string EquipableItem::AC_UNEQUIP = "UNEQUIP";

void EquipableItem::execute(Hero* hero, std::string action)
{
	if (action.compare(AC_EQUIP) == 0) 
	{
		doEquip(hero);
	}
	else if (action.compare(AC_UNEQUIP) == 0) 
	{
		doUnequip(hero, true);
	}
	else 
	{
		Item::execute(hero, action);
	}
}

void EquipableItem::doDrop(Hero* hero)
{
	if (!isEquipped(hero) || doUnequip(hero, false, false)) 
	{
		Item::doDrop(hero);
	}
}

void EquipableItem::cast(Hero* user, int dst)
{
	if (isEquipped(user)) 
	{
		if (quantity == 1 && !doUnequip(user, false, false)) 
		{
			return;
		}
	}

	Item::cast(user, dst);
}

bool EquipableItem::doUnequip(Hero* hero, bool collect, bool single)
{
	if (cursed) 
	{
		GLog::w(TXT_UNEQUIP_CURSED.c_str(), Name());
		return false;
	}

	if (single) 
	{
		hero->spendAndNext(time2equip(hero));
	}
	else 
	{
		//hero->spend(time2equip(hero));
	}

	//if (collect && !collect(hero.belongings.backpack)) {
	//	Dungeon.level.drop(this, hero.pos);
	//}

	return true;
}

bool EquipableItem::doUnequip(Hero* hero, bool collect)
{
	return doUnequip(hero, collect, true);
}

void EquipableItem::equipCursed(Hero* hero)
{
	//hero.sprite.emitter().burst(ShadowParticle.CURSE, 6);
	//Sample.INSTANCE.play(Assets.SND_CURSED);
}
