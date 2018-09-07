#pragma once

#include "kindofweapon.h"
#include "itemstatushandler.h"
#include "buff.h"
#include "mob.h"
#include "wndbag.h"
#include "callback.h"

class Wand :public KindOfWeapon{
protected:
	class Charger :public Buff{
	public:
		virtual std::string getClassName() { return "Charger"; }
		virtual std::string toString() { return "Charger"; };
		virtual bool attachTo(Char* target) {
			Buff::attachTo(target);
			delay();

			return true;
		}

		virtual bool act();

		Wand* w;
		Charger(Wand* wa) :w(wa){}
	protected:
		void delay();
	private:
		static const float TIME_TO_CHARGE;
	};
private:
	static const int USAGES_TO_KNOW = 40;
	static const std::string TXT_WOOD;
	static const std::string TXT_DAMAGE;
	static const std::string TXT_WEAPON;

	static const std::string TXT_FIZZLES;
	static const std::string TXT_SELF_TARGET;

	static const std::string TXT_IDENTIFY;

	static const float TIME_TO_ZAP;

	static const std::string _woods[];
	static std::vector<std::string> woods;

	static const int _images[];
	static std::vector<int> images;

	static std::string _wands[];
	static std::vector<std::string> wands;

	static ItemStatusHandler* handler;

	bool curChargeKnown;

	int usagesToKnow;

	static const std::string UNFAMILIRIARITY;
	static const std::string MAX_CHARGES;
	static const std::string CUR_CHARGES;
	static const std::string CUR_CHARGE_KNOWN;

	std::string wood;
public:
	int maxCharges;
	int curCharges = maxCharges;

	static const std::string AC_ZAP;

	Wand();

	virtual int min() {
		int tier = 1 + effectiveLevel() / 3;
		return tier;
	}
	virtual int max() {
		int level = effectiveLevel();
		int tier = 1 + level / 3;
		return (tier * tier - tier + 10) / 2 + level;
	}

	static void initWoods();
	static void save(Bundle* bundle);
	static void restore(Bundle* bundle);

	virtual void actions(Hero* hero, std::vector<std::string>& actions);
	virtual bool doUnequip(Hero* hero, bool collect, bool single);
	virtual void activate(Hero* hero);

	virtual void execute(Hero* hero, const std::string& action);
	virtual bool collect(Bag* container);

	void charge(Char* owner);
	void onDetach() {
		stopCharging();
	}
	void stopCharging() {
		if (charger != NULL) 
		{
			charger->detach();
			charger = NULL;
		}
	}
	int power();
	void setKnown();
	virtual Item* identify();
	virtual std::string toString();
	virtual std::string Name() {
		return isKnown() ? name : wood + " wand";
	}
	virtual std::string info();
	virtual bool isIdentified();
	virtual std::string status();
	virtual Item* upgrade();
	virtual Item* degrade();
	virtual int maxDurability(int lvl) {
		return 6 * (lvl < 16 ? 16 - lvl : 1);
	}
	virtual Item* random();
	static bool allKnown(); 
	virtual int price() {
		return considerState(50);
	}
	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);
protected:
	Charger* charger;
	bool hitChars;
	static CellSelector::Listener* zapper;

	virtual void init();
	virtual int initialCharges() { return 2; }

	virtual void onZap(int cell) = 0;
	bool isKnown();
	void updateLevel();
	virtual void fx(int cell, Callback* callback);
	void wandUsed();
};

class WandOfTeleportation :public Wand{
public:
	WandOfTeleportation();

	virtual std::string desc() {
		return BPT::getText("lang.WandOfTeleportation_Desc");
			//std::string("A blast from this wand will teleport a creature against ") +
			//std::string("its will to a random place on the current level.");
	}
	virtual std::string getClassName() { return "WandOfTeleportation"; }
	static Item* Create(){ return new WandOfTeleportation(); }
protected:
	virtual void onZap(int cell);
	void fx(int cell, Callback* callback);
};

class WandOfSlowness :public Wand{
public:
	WandOfSlowness();
	virtual std::string desc() {
		return BPT::getText("lang.WandOfSlowness_Desc");
			//std::string("This wand will cause a creature to move and attack ") +
			//std::string("at half its ordinary speed until the effect ends");
	}
	virtual std::string getClassName() { return "WandOfSlowness"; }
	static Item* Create(){ return new WandOfSlowness(); }
protected:
	virtual void onZap(int cell);
	void fx(int cell, Callback* callback);
};

class WandOfFirebolt :public Wand{
public:
	WandOfFirebolt();
	virtual std::string desc() {
		return BPT::getText("lang.WandOfFirebolt_Desc");
			//std::string("This wand unleashes bursts of magical fire. It will ignite ") +
			//std::string("flammable terrain, and will damage and burn a creature it hits.");
	}
	virtual std::string getClassName() { return "WandOfFirebolt"; }
	static Item* Create(){ return new WandOfFirebolt(); }
protected:
	virtual void onZap(int cell);
	void fx(int cell, Callback* callback);
};

class WandOfPoison :public Wand{
public:
	WandOfPoison();

	virtual std::string desc() {
		return BPT::getText("lang.WandOfPoison_Desc");
			//std::string("The vile blast of this twisted bit of wood will imbue its target ") +
			//std::string("with a deadly venom. A creature that is poisoned will suffer periodic ") +
			//std::string("damage until the effect ends. The duration of the effect increases ") +
			//std::string("with the level of the staff.");
	}
	virtual std::string getClassName() { return "WandOfPoison"; }
	static Item* Create(){ return new WandOfPoison(); }
protected:
	virtual void onZap(int cell);
	void fx(int cell, Callback* callback);
};

class WandOfRegrowth :public Wand{
public:
	WandOfRegrowth();
	virtual std::string desc() {
		return BPT::getText("lang.WandOfRegrowth_Desc");
			//std::string("\"When life ceases new life always begins to grow... The eternal cycle always remains!\"");
	}
	virtual std::string getClassName() { return "WandOfRegrowth"; }
	static Item* Create(){ return new WandOfRegrowth(); }
protected:
	virtual void onZap(int cell);
	void fx(int cell, Callback* callback);
};

class WandOfBlink :public Wand{
public:
	WandOfBlink();

	static void appear(Char* ch, int pos);
	virtual std::string desc() {
		return BPT::getText("lang.WandOfBlink_Desc");
			//std::string("This wand will allow you to teleport in the chosen direction. ") +
			//std::string("Creatures and inanimate obstructions will block the teleportation.");
	}
	virtual std::string getClassName() { return "WandOfBlink"; }
	static Item* Create(){ return new WandOfBlink(); }
protected:
	virtual void onZap(int cell);
	void fx(int cell, Callback* callback);
};

class WandOfLightning :public Wand{
public:
	WandOfLightning();
	virtual std::string getClassName() { return "WandOfLightning"; }
	static Item* Create(){ return new WandOfLightning(); }

	virtual std::string desc() {
		return BPT::getText("lang.WandOfLightning_Desc");
			//std::string("This wand conjures forth deadly arcs of electricity, which deal damage ") +
			//std::string("to several creatures standing close to each other.");
	}

private:
	std::list<Char*> affected;

	std::vector<int> points;
	int nPoints;

	void hit(Char* ch, int damage);

	
protected:
	virtual void onZap(int cell);
	void fx(int cell, Callback* callback);
};

class WandOfAmok :public Wand{
public:
	WandOfAmok();

	virtual std::string desc() {
		return BPT::getText("lang.WandOfAmok_Desc");
			//std::string("The purple light from this wand will make the target run amok ") +
			//std::string("attacking random creatures in its vicinity.");
	}
	virtual std::string getClassName() { return "WandOfAmok"; }
	static Item* Create(){ return new WandOfAmok(); }
protected:
	virtual void onZap(int cell);
	void fx(int cell, Callback* callback);
};

class WandOfReach :public Wand{
public:
	WandOfReach();

	virtual std::string desc() {
		return
			std::string("This utility wand can be used to grab objects from a distance and to switch places with enemies. ") +
			std::string("Waves of magic force radiated from it will affect all cells on their way triggering traps, ") +
			std::string("trampling high vegetation, opening closed doors and closing open ones.");
	}
	virtual std::string getClassName() { return "WandOfReach"; }
	static Item* Create(){ return new WandOfReach(); }
private:
	static const std::string TXT_YOU_NOW_HAVE;

	void transport(Heap* heap);
protected:
	virtual void onZap(int cell);
	void fx(int cell, Callback* callback);
};

class WandOfFlock :public Wand{
public:
	class Sheep :public NPC{
	public:
		Sheep();

		virtual CharSprite* Sprite();
		virtual std::string getClassName() { return "Sheep"; }
		float lifespan;

		virtual void damage(int dmg, const std::string& src) {
		}
		virtual std::string description() {
			return BPT::getText("lang.WandOfFlock_Sheep_Desc");
				//std::string("This is a magic sheep. What's so magical about it? You can't kill it. ") +
				//std::string("It will stand there until it magcially fades away, all the while chewing cud with a blank stare.");
		}
		virtual void interact();
	protected:
		virtual bool act();
	private:
		static const std::string QUOTES[4];
		bool initialized;
	};
public:
	WandOfFlock();

	virtual std::string desc() {
		return BPT::getText("lang.WandOfFlock_Desc");
			//std::string("A flick of this wand summons a flock of magic sheep, creating temporary impenetrable obstacle.");
	}
	virtual std::string getClassName() { return "WandOfFlock"; }
	static Item* Create(){ return new WandOfFlock(); }
protected:
	virtual void onZap(int cell);
	void fx(int cell, Callback* callback);
};

class WandOfDisintegration :public Wand{
public:
	WandOfDisintegration();

	virtual std::string desc() {
		return BPT::getText("lang.WandOfDisintegration_Desc");
			//std::string("This wand emits a beam of destructive energy, which pierces all creatures in its way. ") +
			//std::string("The more targets it hits, the more damage it inflicts to each of them.");
	}
	virtual std::string getClassName() { return "WandOfDisintegration"; }
	static Item* Create(){ return new WandOfDisintegration(); }
private:
	int distance() {
		return Level() + 4;
	}
protected:
	virtual void onZap(int cell);
	void fx(int cell, Callback* callback);
};

class WandOfAvalanche :public Wand{
public:
	WandOfAvalanche();

	virtual std::string desc() {
		return BPT::getText("lang.WandOfAvalanche_Desc");
			//std::string ("When a discharge of this wand hits a wall (or any other solid obstacle) it causes ") +
			//std::string("an avalanche of stones, damaging and stunning all creatures in the affected area.");
	}
	virtual std::string getClassName() { return "WandOfAvalanche"; }
	static Item* Create(){ return new WandOfAvalanche(); }
protected:
	virtual void onZap(int cell);
	void fx(int cell, Callback* callback);
};

class WandOfMagicMissile :public Wand{
public:
	WandOfMagicMissile();

	virtual std::string getClassName() { return "WandOfMagicMissile"; }
	static Item* Create(){ return new WandOfMagicMissile(); }

	static const String AC_DISENCHANT;

	virtual void actions(Hero* hero, std::vector<std::string>& actions);
	virtual void execute(Hero* hero, const std::string& action);
	virtual void setKnown() {
	}
	virtual String desc() {
		return BPT::getText("lang.WandOfMagicMissile_Desc");
			//"This wand launches missiles of pure magical energy, dealing moderate damage to a target creature.";
	}

	static const String TXT_DISENCHANTED;
	static const float TIME_TO_DISENCHANT;

	boolean disenchantEquipped;
private:
	static const String TXT_SELECT_WAND;
	
	WndBag::Listener* itemSelector;
protected:
	virtual void onZap(int cell);
	virtual boolean isKnown() {
		return true;
	}
	virtual int initialCharges() {
		return 3;
	}
};