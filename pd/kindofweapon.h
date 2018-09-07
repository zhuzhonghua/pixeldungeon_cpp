#pragma once

#include "equipableitem.h"

class KindOfWeapon :public EquipableItem{
private:
	static const std::string TXT_EQUIP_CURSED;

protected:
	static const float TIME_TO_EQUIP;

public:
	virtual void actions(Hero* hero, std::vector<std::string>& actions);
	virtual bool isEquipped(Hero* hero);
	virtual bool doEquip(Hero* hero);
	virtual bool doUnequip(Hero* hero, bool collect, bool single);
	virtual void activate(Hero* hero) { }

	virtual int min() = 0;
	virtual int max() = 0;

	virtual int damageRoll(Hero* owner);
	virtual float acuracyFactor(Hero* hero) { return 1.0f; }
	virtual float speedFactor(Hero* hero) { return 1.0f; }
	virtual void proc(Char* attacker, Char* defender, int damage) {}
};