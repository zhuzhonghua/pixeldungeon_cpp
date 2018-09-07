#pragma once

#include "item.h"

class EquipableItem :public Item{
private:
	static const std::string TXT_UNEQUIP_CURSED;

public:
	static const std::string AC_EQUIP;
	static const std::string AC_UNEQUIP;

	virtual void execute(Hero* hero, std::string action);
	virtual void doDrop(Hero* hero);
	virtual void cast(Hero* user, int dst);
	virtual bool doEquip(Hero* hero) = 0;

	virtual bool doUnequip(Hero* hero, bool collect, bool single);
	virtual bool doUnequip(Hero* hero, bool collect);

protected:
	static void equipCursed(Hero* hero);
	virtual float time2equip(Hero* hero) { return 1; }
};