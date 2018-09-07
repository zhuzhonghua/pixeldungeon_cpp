#pragma once

#include "kindofweapon.h"
#include "typedefine.h"
#include "wndbag.h"
#include "bpt.h"

class Weapon :public KindOfWeapon{
public:
	class Enchantment :public Bundlable{
	private:
		static std::string _enchants[];
		static std::vector<std::string> enchants;
		static float _chances[];
		static std::vector<float> chances;

		typedef Enchantment* (*CreateEnchant)();
		static std::map<std::string, CreateEnchant> clsToInst;

	public:
		virtual boolean proc(Weapon* weapon, Char* attacker, Char* defender, int damage) = 0;
		virtual String name(const String& weaponName) {
			return weaponName;
		}
		virtual void restoreFromBundle(Bundle* bundle) {
		}
		virtual void storeInBundle(Bundle* bundle) {
		}
		virtual ItemSprite::Glowing* glowing();

		static Enchantment* random();
		static void init();
	};
private:
	static const int HITS_TO_KNOW = 20;

	static const String TXT_IDENTIFY;
	static const String TXT_INCOMPATIBLE;

	static const String TXT_TO_STRING;
	static const String TXT_BROKEN;
	static const String UNFAMILIRIARITY;
	static const String ENCHANTMENT;
	static const String IMBUE;

	int hitsToKnow;
public:
	int		STR;
	float	ACU;
	float	DLY;

	enum Imbue {
		NONE, SPEED, ACCURACY
	};
	Imbue imbue;

	Weapon();
	virtual void proc(Char* attacker, Char* defender, int damage);

	virtual std::string getClassName() { return "Weapon"; }

	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);
	virtual float acuracyFactor(Hero* hero);
	virtual float speedFactor(Hero* hero);
	virtual int damageRoll(Hero* owner);

	Item* upgrade(boolean enchant);

	virtual int maxDurability(int lvl) {
		return 5 * (lvl < 16 ? 16 - lvl : 1);
	}
	virtual String toString();
	virtual String Name();
	virtual Item* random();
	Weapon* enchant(Enchantment* ench);
	Weapon* enchant();
	boolean isEnchanted() {
		return enchantment != NULL;
	}
	virtual ItemSprite::Glowing* glowing();
protected:
	Enchantment* enchantment;
};

class EnchantmentFire :public Weapon::Enchantment{
private:
	static const String TXT_BLAZING;

	static ItemSprite::Glowing* ORANGE;

public:
	virtual boolean proc(Weapon* weapon, Char* attacker, Char* defender, int damage);
	virtual ItemSprite::Glowing* glowing() { return ORANGE; }
	virtual String name(const String& weaponName);

	virtual std::string getClassName() { return "EnchantmentFire"; }
	static Weapon::Enchantment* Create() { return new EnchantmentFire(); }
};

class EnchantmentPoison :public Weapon::Enchantment{
private:
	static const String TXT_VENOMOUS;

	static ItemSprite::Glowing* PURPLE;

public:
	virtual boolean proc(Weapon* weapon, Char* attacker, Char* defender, int damage);
	virtual ItemSprite::Glowing* glowing() { return PURPLE; }
	virtual String name(const String& weaponName);

	virtual std::string getClassName() { return "EnchantmentPoison"; }
	static Weapon::Enchantment* Create() { return new EnchantmentPoison(); }
};

class Death :public Weapon::Enchantment{
private:
	static const String TXT_GRIM;

	static ItemSprite::Glowing* BLACK;

public:
	virtual boolean proc(Weapon* weapon, Char* attacker, Char* defender, int damage);
	virtual ItemSprite::Glowing* glowing() { return BLACK; }
	virtual String name(const String& weaponName);

	virtual std::string getClassName() { return "Death"; }
	static Weapon::Enchantment* Create() { return new Death(); }
};

class EnchantmentParalysis :public Weapon::Enchantment{
private:
	static const String TXT_STUNNING;

	static ItemSprite::Glowing* YELLOW;

public:
	virtual boolean proc(Weapon* weapon, Char* attacker, Char* defender, int damage);
	virtual ItemSprite::Glowing* glowing() { return YELLOW; }
	virtual String name(const String& weaponName);

	virtual std::string getClassName() { return "EnchantmentParalysis"; }
	static Weapon::Enchantment* Create() { return new EnchantmentParalysis(); }
};

class Leech :public Weapon::Enchantment{
private:
	static const String TXT_VAMPIRIC;

	static ItemSprite::Glowing* RED;

public:
	virtual boolean proc(Weapon* weapon, Char* attacker, Char* defender, int damage);
	virtual ItemSprite::Glowing* glowing() { return RED; }
	virtual String name(const String& weaponName);

	virtual std::string getClassName() { return "Leech"; }
	static Weapon::Enchantment* Create() { return new Leech(); }
};

class EnchantmentSlow :public Weapon::Enchantment{
private:
	static const String TXT_CHILLING;

	static ItemSprite::Glowing* BLUE;

public:
	virtual boolean proc(Weapon* weapon, Char* attacker, Char* defender, int damage);
	virtual ItemSprite::Glowing* glowing() { return BLUE; }
	virtual String name(const String& weaponName);

	virtual std::string getClassName() { return "EnchantmentSlow"; }
	static Weapon::Enchantment* Create() { return new EnchantmentSlow(); }
};

class Shock :public Weapon::Enchantment{
private:
	static const String TXT_SHOCKING;

	List<Char*> affected;
	int points[20];;
	int nPoints;

	void hit(Char* ch, int damage);
public:
	virtual boolean proc(Weapon* weapon, Char* attacker, Char* defender, int damage);
	virtual String name(const String& weaponName);

	virtual std::string getClassName() { return "Shock"; }
	static Weapon::Enchantment* Create() { return new Shock(); }
};

class Instability :public Weapon::Enchantment{
private:
	static const String TXT_UNSTABLE;
public:
	virtual boolean proc(Weapon* weapon, Char* attacker, Char* defender, int damage);
	virtual String name(const String& weaponName);

	virtual std::string getClassName() { return "Instability"; }
	static Weapon::Enchantment* Create() { return new Instability(); }
};

class Horror :public Weapon::Enchantment{
private:
	static const String TXT_ELDRITCH;

	static ItemSprite::Glowing* GREY;

public:
	virtual boolean proc(Weapon* weapon, Char* attacker, Char* defender, int damage);
	virtual ItemSprite::Glowing* glowing() { return GREY; }
	virtual String name(const String& weaponName);

	virtual std::string getClassName() { return "Horror"; }
	static Weapon::Enchantment* Create() { return new Horror(); }
};

class Luck :public Weapon::Enchantment{
private:
	static const String TXT_LUCKY;

	static ItemSprite::Glowing* GREEN;

public:
	virtual std::string getClassName() { return "Luck"; }

	virtual boolean proc(Weapon* weapon, Char* attacker, Char* defender, int damage);
	virtual ItemSprite::Glowing* glowing() { return GREEN; }
	virtual String name(const String& weaponName);

	static Weapon::Enchantment* Create() { return new Luck(); }
};

class Tempering :public Weapon::Enchantment{
private:
	static const String TXT_TEMPERED;

	static ItemSprite::Glowing* GRAY;

public:
	virtual std::string getClassName() { return "Tempering"; }

	virtual boolean proc(Weapon* weapon, Char* attacker, Char* defender, int damage);
	virtual ItemSprite::Glowing* glowing() { return GRAY; }
	virtual String name(const String& weaponName);

	static Weapon::Enchantment* Create() { return new Tempering(); }
};

class MeleeWeapon :public Weapon{
public:
	MeleeWeapon(int tier, float acu, float dly);

	virtual std::string getClassName() { return "MeleeWeapon"; }

	virtual int min() {
		return isBroken() ? min0() : min0() + Level();
	}
	virtual int max() {
		return isBroken() ? max0() : max0() + Level() * tier;
	}
	virtual Item* upgrade() {
		return upgrade(false);
	}

	Item* upgrade(boolean enchant) {
		STR--;
		return Weapon::upgrade(enchant);
	}
	Item* safeUpgrade() {
		return upgrade(enchantment != NULL);
	}

	virtual Item* degrade() {
		STR++;
		return Weapon::degrade();
	}

	int typicalSTR() {
		return 8 + tier * 2;
	}
	virtual String info();
	virtual int price();

	virtual Item* random();
private:
	int tier;

protected:
	virtual int min0() { return tier; }

	virtual int max0() { return (int)((tier * tier - tier + 10) / ACU * DLY); }
};

class Dagger :public MeleeWeapon{
public:
	Dagger();

	virtual String desc() {
		//return "A simple iron dagger with a well worn wooden handle.";
		return BPT::getText("lang.dagger_desc");
	}

	virtual std::string getClassName() { return "Dagger"; }
	static Item* Create() { return new Dagger(); }
};

class Knuckles :public MeleeWeapon{
public:
	Knuckles();
	virtual String desc() {
		//return "A piece of iron shaped to fit around the knuckles.";
		return BPT::getText("lang.knuckles_desc");
	}

	virtual std::string getClassName() { return "Knuckles"; }
	static Item* Create() { return new Knuckles(); }
};

class Quarterstaff :public MeleeWeapon{
public:
	Quarterstaff();
	virtual String desc() {
		//return "A staff of hardwood, its ends are shod with iron.";
		return BPT::getText("lang.quarterstaff_desc");
	}
	virtual std::string getClassName() { return "Quarterstaff"; }
	static Item* Create() { return new Quarterstaff(); }
};

class Spear :public MeleeWeapon{
public:
	Spear();
	virtual String desc() {
		//return "A slender wooden rod tipped with sharpened iron.";
		return BPT::getText("lang.spear_desc");
	}
	virtual std::string getClassName() { return "Spear"; }
	static Item* Create() { return new Spear(); }
};

class Mace :public MeleeWeapon{
public:
	Mace();
	virtual String desc() {
		//return "The iron head of this weapon inflicts substantial damage.";
		return BPT::getText("lang.mace_desc");
	}
	virtual std::string getClassName() { return "Mace"; }
	static Item* Create() { return new Mace(); }
};

class Sword :public MeleeWeapon{
public:
	Sword();

	virtual String desc() {
		//return "The razor-sharp length of steel blade shines reassuringly.";
		return BPT::getText("lang.sword_desc");
	}
	virtual std::string getClassName() { return "Sword"; }
	static Item* Create() { return new Sword(); }
};

class Longsword :public MeleeWeapon{
public:
	Longsword();
	virtual String desc() {
		//return "This towering blade inflicts heavy damage by investing its heft into every cut.";
		return BPT::getText("lang.longsword_desc");
	}
	virtual std::string getClassName() { return "Longsword"; }
	static Item* Create() { return new Longsword(); }
};

class BattleAxe :public MeleeWeapon{
public:
	BattleAxe();

	virtual String desc() {
		//return "The enormous steel head of this battle axe puts considerable heft behind each stroke.";
		return BPT::getText("lang.battleaxe_desc");
	}
	virtual std::string getClassName() { return "BattleAxe"; }
	static Item* Create() { return new BattleAxe(); }
};

class WarHammer :public MeleeWeapon{
public:
	WarHammer();
	virtual String desc() {
		//return
		//	std::string("Few creatures can withstand the crushing blow of this towering mass of lead and steel, ") +
		//	std::string("but only the strongest of adventurers can use it effectively.");
		return BPT::getText("lang.warhammer_desc");
	}

	virtual std::string getClassName() { return "WarHammer"; }
	static Item* Create() { return new WarHammer(); }
};

class Glaive :public MeleeWeapon{
public:
	Glaive();
	virtual String desc() {
		//return "A polearm consisting of a sword blade on the end of a pole.";
		return BPT::getText("lang.glaive_desc");
	}

	virtual std::string getClassName() { return "Glaive"; }
	static Item* Create() { return new Glaive(); }
};

class ShortSword :public MeleeWeapon{
public:
	static const String AC_REFORGE;
	static const String TXT_SELECT_WEAPON;

	static const String TXT_REFORGED;
	static const String TXT_NOT_BOOMERANG;

	static const float TIME_TO_REFORGE;

	boolean  equipped;

private:
	WndBag::Listener* itemSelector;
public:
	ShortSword();

	virtual void actions(Hero* hero, std::vector<std::string>& actions);
	virtual void execute(Hero* hero, std::string action);
	virtual String desc() {
		//return
		//	"It is indeed quite short, just a few inches longer, than a dagger.";
		return BPT::getText("lang.shortsword_desc");
	}

	virtual std::string getClassName() { return "ShortSword"; }
	static Item* Create() { return new ShortSword(); }
protected:
	virtual int max0() {
		return 12;
	}
};

class MissileWeapon :public Weapon{
private:
	static const String TXT_MISSILES;
	static const String TXT_YES;
	static const String TXT_NO;
	static const String TXT_R_U_SURE;

public:
	MissileWeapon();

	virtual void actions(Hero* hero, std::vector<std::string>& actions);
	virtual void proc(Char* attacker, Char* defender, int damage);
	virtual bool doEquip(Hero* hero);
	virtual Item* random() {
		return this;
	}
	virtual boolean isUpgradable() {
		return false;
	}
	virtual boolean isIdentified() {
		return true;
	}
	virtual String info();
protected:
	virtual void onThrow(int cell);
	virtual void miss(int cell) {
		Weapon::onThrow(cell);
	}
};

class Boomerang :public MissileWeapon{
public:
	Boomerang();

	virtual int min() {
		return isBroken() ? 1 : 1 + Level();
	}
	virtual int max() {
		return isBroken() ? 4 : 4 + 2 * Level();
	}
	virtual boolean isUpgradable() {
		return true;
	}
	virtual Item* upgrade() {
		return upgrade(false);
	}
	virtual Item* upgrade(boolean enchant) {
		MissileWeapon::upgrade(enchant);

		updateQuickslot();

		return this;
	}
	virtual int maxDurability(int lvl) {
		return 8 * (lvl < 16 ? 16 - lvl : 1);
	}
	virtual void proc(Char* attacker, Char* defender, int damage);
	virtual void cast(Hero* user, int dst);

	virtual String desc() {
		//return
		//	"Thrown to the enemy this flat curved wooden missile will return to the hands of its thrower.";
		return BPT::getText("lang.boomerang_desc");
	}
	virtual std::string getClassName() { return "Boomerang"; }
	static Item* Create() { return new Boomerang(); }
protected:
	virtual void miss(int cell) {
		circleBack(cell, curUser);
	}

private:
	boolean throwEquiped;
	void circleBack(int from, Hero* owner);
};

class Dart :public MissileWeapon{
public:
	Dart();
	Dart(int number);
	virtual std::string getClassName() { return "Dart"; }
	static Item* Create() { return new Dart(); }

	virtual int min() {
		return 1;
	}
	virtual int max() {
		return 4;
	}
	virtual String desc() {
		//return
		//	std::string("These simple metal spikes are weighted to fly true and ") +
		//	std::string("sting their prey with a flick of the wrist.");
		return BPT::getText("lang.dart_desc");
	}
	virtual Item* random();
	virtual int price() {
		return quantity * 2;
	}
};

class Javelin :public MissileWeapon{
public:
	Javelin();
	Javelin(int number);
	virtual std::string getClassName() { return "Javelin"; }
	static Item* Create() { return new Javelin(); }

	virtual int min() {
		return 2;
	}
	virtual int max() {
		return 15;
	}
	virtual void proc(Char* attacker, Char* defender, int damage);
	virtual String desc() {
		//return
		//	std::string("This length of metal is weighted to keep the spike ") +
		//	std::string("at its tip foremost as it sails through the air.");
		return BPT::getText("lang.javelin_desc");
	}
	virtual Item* random();
	virtual int price() {
		return 15 * quantity;
	}
};

class IncendiaryDart :public MissileWeapon{
public:
	IncendiaryDart();
	IncendiaryDart(int number);
	virtual std::string getClassName() { return "IncendiaryDart"; }
	static Item* Create() { return new IncendiaryDart(); }

	virtual int min() {
		return 1;
	}
	virtual int max() {
		return 2;
	}
	virtual void proc(Char* attacker, Char* defender, int damage);
	virtual String desc() {
		//return
		//	std::string("The spike on each of these darts is designed to pin it to its target ") +
		//	std::string("while the unstable compounds strapped to its length burst into brilliant flames.");
		return BPT::getText("lang.incendiarydart_desc");
	}
	virtual Item* random();
	virtual int price() {
		return 10 * quantity;
	}

protected:
	virtual void onThrow(int cell);
};

class CurareDart :public MissileWeapon{
public:
	static const float DURATION;
	CurareDart();
	CurareDart(int number);
	virtual std::string getClassName() { return "CurareDart"; }
	static Item* Create() { return new CurareDart(); }

	virtual int min() {
		return 1;
	}
	virtual int max() {
		return 3;
	}

	virtual void proc(Char* attacker, Char* defender, int damage);

	virtual String desc() {
		//return
		//	std::string("These little evil darts don't do much damage but they can paralyze ") +
		//	std::string("the target leaving it helpless and motionless for some time.");
		return BPT::getText("lang.curaredart_desc");
	}
	virtual Item* random();
	virtual int price() {
		return 12 * quantity;
	}
};

class Shuriken :public MissileWeapon{
public:
	Shuriken();
	Shuriken(int number);
	virtual std::string getClassName() { return "Shuriken"; }
	static Item* Create() { return new Shuriken(); }

	virtual int min() {
		return 2;
	}

	virtual int max() {
		return 6;
	}

	virtual String desc() {
		//return
		//	std::string("Star-shaped pieces of metal with razor-sharp blades do significant damage ") +
		//	std::string("when they hit a target. They can be thrown at very high rate.");
		return BPT::getText("lang.shuriken_desc");
	}
	virtual Item* random();
	virtual int price() {
		return 15 * quantity;
	}
};

class Tamahawk :public MissileWeapon{
public:
	Tamahawk();
	Tamahawk(int number);
	virtual std::string getClassName() { return "Tamahawk"; }
	static Item* Create() { return new Tamahawk(); }

	virtual int min() {
		return 4;
	}

	virtual int max() {
		return 20;
	}

	virtual void proc(Char* attacker, Char* defender, int damage);
	virtual String desc() {
		//return
		//	std::string("This throwing axe is not that heavy, but it still ") +
		//	std::string("requires significant strength to be used effectively.");
		return BPT::getText("lang.tamahawk_desc");
	}
	virtual Item* random();
	virtual int price() {
		return 20 * quantity;
	}
};