#pragma once

#include "equipableitem.h"
#include "buff.h"
#include "itemstatushandler.h"

class Ring :public EquipableItem{
public:
	class RingBuff :public Buff{
	private:
		static const std::string TXT_KNOWN;

	public:
		Ring* ring;
		int level;

		RingBuff(Ring* r);

		virtual std::string getClassName(){ return className(); }
		static std::string className() { return "RingBuff"; }

		virtual bool attachTo(Char* target);
		virtual bool act();
	};
private:
	static const int TICKS_TO_KNOW = 200;

	static const float TIME_TO_EQUIP;

	static const char* TXT_IDENTIFY;

	static const char* TXT_UNEQUIP_TITLE;
	static const std::string TXT_UNEQUIP_MESSAGE;
	static std::string _gems[];
	static std::vector<std::string> gems;

	static int _images[];
	static std::vector<int> images;

	static std::string _rings[];
	static std::vector<std::string> rings;

	std::string gem;

	int ticksToKnow;

	static const std::string UNFAMILIRIARITY;

	static ItemStatusHandler* handler;

	void renewBuff();
protected:
	Buff* buff;

	void setKnown();
	virtual RingBuff* Buff() { return NULL; }
public:
	Ring();

	static void initGems();
	static void save(Bundle* bundle);
	static void restore(Bundle* bundle);

	void syncGem();

	virtual void actions(Hero* hero, std::vector<std::string>& actions);
	virtual bool doEquip(Hero* hero);

	void activate(Char* ch);

	virtual bool doUnequip(Hero* hero, bool collect, bool single);
	virtual bool isEquipped(Hero* hero);
	virtual int effectiveLevel();

	virtual void getBroken();
	virtual void fix();

	virtual int maxDurability(int lvl);
	virtual bool isKnown();

	virtual std::string toString();

	virtual std::string Name();
	virtual std::string desc();
	virtual std::string info();
	virtual bool isIdentified();

	virtual Item* identify();
	virtual Item* random();

	static bool allKnown();
	virtual int price();

	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);
	virtual std::string getClassName() { return "Ring"; }
};

class RingOfMending :public Ring{
public:
	class Rejuvenation :public RingBuff 
	{
	public:
		Rejuvenation(Ring* r) :RingBuff(r){}

		virtual std::string getClassName(){ return className(); }
		static std::string className() { return "Rejuvenation"; }
		virtual std::string toString() { return ""; }
	};

	RingOfMending();
	virtual std::string desc();

	virtual std::string getClassName() { return "RingOfMending"; }
	static Item* Create() { return new RingOfMending(); }
protected:
	virtual RingBuff* Buff(){ return new Rejuvenation(this); }
};

class RingOfDetection :public Ring{
public:
	class Detection :public RingBuff {
	public:
		Detection(Ring* r) :RingBuff(r){}
		virtual std::string getClassName(){ return className(); }
		static std::string className() { return "Detection"; }
	};
	RingOfDetection();

	virtual bool doEquip(Hero* hero);

	virtual std::string getClassName() { return "RingOfDetection"; }
	static Item* Create() { return new RingOfDetection(); }
};

class RingOfShadows :public Ring{
public:
	class Shadows :public RingBuff {
	public:
		Shadows(Ring* r) :RingBuff(r){}
		virtual std::string getClassName(){ return className(); }
		static std::string className() { return "Shadows"; }
		virtual std::string toString() { return ""; }
	};

	RingOfShadows();
	virtual std::string getClassName() { return "RingOfShadows"; }
	static Item* Create() { return new RingOfShadows(); }
	virtual std::string desc();
protected:
	virtual RingBuff* Buff() {
		return new Shadows(this);
	}
};

class RingOfPower :public Ring{
public:
	class Power :public RingBuff {
	public:
		Power(Ring* r) :RingBuff(r){}
		virtual std::string getClassName(){ return className(); }
		static std::string className() { return "Power"; }
		virtual std::string toString() { return ""; }
	};

	RingOfPower(){ name = "Ring of Power"; }
	virtual std::string getClassName() { return "RingOfPower"; }
	static Item* Create() { return new RingOfPower(); }
	virtual std::string desc();
protected:
	virtual RingBuff* Buff() {
		return new Power(this);
	}
};

class RingOfHerbalism :public Ring{
public:
	class Herbalism :public RingBuff {
	public:
		Herbalism(Ring* r) :RingBuff(r){}
		virtual std::string getClassName(){ return className(); }
		static std::string className() { return "Herbalism"; }
		virtual std::string toString() { return ""; }
	};

	RingOfHerbalism(){ name = "Ring of Herbalism"; }
	virtual std::string getClassName() { return "RingOfHerbalism"; }
	static Item* Create() { return new RingOfHerbalism(); }
	virtual std::string desc();
protected:
	virtual RingBuff* Buff() {
		return new Herbalism(this);
	}
};

class RingOfAccuracy :public Ring{
public:
	class Accuracy :public RingBuff {
	public:
		Accuracy(Ring* r) :RingBuff(r){}
		virtual std::string getClassName(){ return className(); }
		static std::string className() { return "Accuracy"; }
		virtual std::string toString() { return ""; }
	};

	RingOfAccuracy(){ name = "Ring of Accuracy"; }
	virtual std::string getClassName() { return "RingOfAccuracy"; }
	static Item* Create() { return new RingOfAccuracy(); }
	virtual std::string desc();
protected:
	virtual RingBuff* Buff() {
		return new Accuracy(this);
	}
};

class RingOfEvasion :public Ring{
public:
	class Evasion :public RingBuff {
	public:
		Evasion(Ring* r) :RingBuff(r){}
		virtual std::string getClassName(){ return className(); }
		static std::string className() { return "Evasion"; }
		virtual std::string toString() { return ""; }
	};

	RingOfEvasion(){ name = "Ring of Evasion"; }
	virtual std::string getClassName() { return "RingOfEvasion"; }
	static Item* Create() { return new RingOfEvasion(); }
	virtual std::string desc();
protected:
	virtual RingBuff* Buff() {
		return new Evasion(this);
	}
};

class RingOfSatiety :public Ring{
public:
	class Satiety :public RingBuff {
	public:
		Satiety(Ring* r) :RingBuff(r){}
		virtual std::string getClassName(){ return className(); }
		static std::string className() { return "Satiety"; }
		virtual std::string toString() { return ""; }
	};

	RingOfSatiety(){ name = "Ring of Satiety"; }
	virtual std::string getClassName() { return "RingOfSatiety"; }
	static Item* Create() { return new RingOfSatiety(); }
	virtual std::string desc();
protected:
	virtual RingBuff* Buff() {
		return new Satiety(this);
	}
};

class RingOfHaste :public Ring{
public:
	class Haste :public RingBuff {
	public:
		Haste(Ring* r) :RingBuff(r){}
		virtual std::string getClassName(){ return className(); }
		static std::string className() { return "RingOfHaste::Haste"; }
		virtual std::string toString() { return ""; }
	};

	RingOfHaste(){ name = "Ring of Haste"; }
	virtual std::string getClassName() { return "RingOfHaste"; }
	static Item* Create() { return new RingOfHaste(); }
	virtual std::string desc();
protected:
	virtual RingBuff* Buff() {
		return new Haste(this);
	}
};

class RingOfHaggler :public Ring{
public:
	class Haggling :public RingBuff {
	public:
		Haggling(Ring* r) :RingBuff(r){}
		virtual std::string getClassName(){ return className(); }
		static std::string className() { return "RingOfHagglerHaggling"; }
		virtual std::string toString() { return ""; }
	};

	RingOfHaggler(){ name = BPT::getText("lang.Ring_of_Haggler"); }
	virtual std::string getClassName() { return "RingOfHaggler"; }
	static Item* Create() { return new RingOfHaggler(); }
	virtual std::string desc();

	virtual void use() {
		// Do nothing (it can't degrade)
	}
	virtual bool isUpgradable() {
		return false;
	}
	virtual Item* random() {
		Level(+1);
		return this;
	}
	virtual bool doPickUp(Hero* hero);
protected:
	virtual RingBuff* Buff() {
		return new Haggling(this);
	}
};

class RingOfElements :public Ring{
public:
	class Resistance :public RingBuff {
	public:
		Resistance(Ring* r) :RingBuff(r){}
		virtual std::string getClassName(){ return className(); }
		static std::string className() { return "Resistance"; }
		virtual std::string toString() { return ""; }

		float durationFactor() {
			return level < 0 ? 1 : (2 + 0.5f * level) / (2 + level);
		}

		std::set<std::string> resistances();
	};

	RingOfElements();
	virtual std::string getClassName() { return "RingOfElements"; }
	static Item* Create() { return new RingOfElements(); }
	virtual std::string desc();

	
protected:
	virtual RingBuff* Buff() {
		return new Resistance(this);
	}

private:
	static std::set<std::string> EMPTY;
	static std::set<std::string> FULL;
};

class RingOfThorns :public Ring{
public:
	class Thorns :public RingBuff {
	public:
		Thorns(Ring* r) :RingBuff(r){}
		virtual std::string getClassName(){ return className(); }
		static std::string className() { return "Thorns"; }
		virtual std::string toString() { return ""; }
	};

	RingOfThorns(){ name = "Ring of Thorns"; }
	virtual std::string getClassName() { return "RingOfThorns"; }
	static Item* Create() { return new RingOfThorns(); }
	virtual std::string desc();

	virtual void use() {
		// Do nothing (it can't degrade)
	}
	virtual bool isUpgradable() {
		return false;
	}
	virtual Item* random() {
		Level(+1);
		return this;
	}
	virtual bool doPickUp(Hero* hero);
protected:
	virtual RingBuff* Buff() {
		return new Thorns(this);
	}
};