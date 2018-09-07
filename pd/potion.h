#pragma once

#include "item.h"
#include "typedefine.h"
#include "itemstatushandler.h"

class Potion :public Item{
public:
	static const String AC_DRINK;

	virtual std::string getClassName() { return "Potion"; }
private:
	static const String TXT_HARMFUL;
	static const String TXT_BENEFICIAL;
	static const String TXT_YES;
	static const String TXT_NO;
	static const String TXT_R_U_SURE_DRINK;
	static const String TXT_R_U_SURE_THROW;

	static const float TIME_TO_DRINK;

	static String _potions[];

	static std::vector<String> potions;

	static String _colors[];
	static std::vector<String> colors;

	static int _images[];
	static std::vector<int> images;

	static ItemStatusHandler* handler;

	String color;

public:
	Potion();
	static void initColors();
	static void save(Bundle* bundle);
	static void restore(Bundle* bundle);

	virtual void actions(Hero* hero, std::vector<std::string>& actions);
	virtual void execute(Hero* hero, std::string action);
	virtual void doThrow(Hero* hero);
	virtual void shatter(int cell);
	boolean isKnown();
	void setKnown();

	virtual Item* identify() {
		setKnown();
		return this;
	}
	virtual String Name() {
		return isKnown() ? name : (color + " potion");
	}
	virtual String info() {
		return isKnown() ?
			desc() :
			std::string("This flask contains a swirling ") + color + (" liquid. ") +
			std::string("Who knows what it will do when drunk or thrown?");
	}
	virtual boolean isIdentified() {
		return isKnown();
	}
	virtual boolean isUpgradable() {
		return false;
	}
	static std::set<std::string> getKnown() {
		return handler->known;
	}
	static std::set<std::string> getUnknown() {
		return handler->unknown();
	}
	static boolean allKnown() {
		return handler->known.size() == potions.size();
	}
	virtual int price() {
		return 20 * quantity;
	}
protected:
	void drink(Hero* hero);
	virtual void onThrow(int cell);
	virtual void apply(Hero* hero);
	String Color() { return color; }
	void splash(int cell);
};

class PotionOfHealing :public Potion{
public:
	PotionOfHealing();
	virtual std::string getClassName() { return "PotionOfHealing"; }
	static Item* Create(){ return new PotionOfHealing(); }

	static void heal(Hero* hero);
	virtual String desc() {
		return
			"An elixir that will instantly return you to full health and cure poison.";
	}
	virtual int price() {
		return isKnown() ? 30 * quantity : Potion::price();
	}
protected:
	virtual void apply(Hero* hero);
};

class PotionOfExperience :public Potion{
public:
	PotionOfExperience();
	virtual std::string getClassName() { return "PotionOfExperience"; }
	static Item* Create(){ return new PotionOfExperience(); }

	virtual String desc() {
		return
			String("The storied experiences of multitudes of battles reduced to liquid form, ") +
			String("this draught will instantly raise your experience level.");
	}
	virtual int price() {
		return isKnown() ? 80 * quantity : Potion::price();
	}
protected:
	virtual void apply(Hero* hero);
};

class PotionOfToxicGas :public Potion{
public:
	PotionOfToxicGas();
	virtual std::string getClassName() { return "PotionOfToxicGas"; }
	static Item* Create(){ return new PotionOfToxicGas(); }

	virtual void shatter(int cell);

	virtual String desc() {
		return
			String("Uncorking or shattering this pressurized glass will cause ") +
			String("its contents to explode into a deadly cloud of toxic green gas. ") +
			String("You might choose to fling this potion at distant enemies ") +
			String("instead of uncorking it by hand.");
	}
	virtual int price() {
		return isKnown() ? 40 * quantity : Potion::price();
	}
};

class PotionOfLiquidFlame :public Potion{
public:
	PotionOfLiquidFlame();
	virtual std::string getClassName() { return "PotionOfLiquidFlame"; }
	static Item* Create(){ return new PotionOfLiquidFlame(); }

	virtual void shatter(int cell);
	virtual String desc() {
		return
			String("This flask contains an unstable compound which will burst ") +
			String("violently into flame upon exposure to open air.");
	}
	virtual int price() {
		return isKnown() ? 40 * quantity : Potion::price();
	}
};

class PotionOfStrength :public Potion{
public:
	PotionOfStrength();
	virtual std::string getClassName() { return "PotionOfStrength"; }
	static Item* Create(){ return new PotionOfStrength(); }

	virtual String desc() {
		return
			String("This powerful liquid will course through your muscles, ") +
			String("permanently increasing your strength by one point.");
	}
	virtual int price() {
		return isKnown() ? 100 * quantity : Potion::price();
	}
protected:
	virtual void apply(Hero* hero);
};

class PotionOfParalyticGas :public Potion{
public:
	PotionOfParalyticGas();
	virtual std::string getClassName() { return "PotionOfParalyticGas"; }
	static Item* Create(){ return new PotionOfParalyticGas(); }

	virtual void shatter(int cell);

	virtual String desc() {
		return
			String("Upon exposure to open air, the liquid in this flask will vaporize ") +
			String("into a numbing yellow haze. Anyone who inhales the cloud will be paralyzed ") +
			String("instantly, unable to move for some time after the cloud dissipates. This ") +
			String("item can be thrown at distant enemies to catch them within the effect of the gas.");
	}
	virtual int price() {
		return isKnown() ? 40 * quantity : Potion::price();
	}
};

class PotionOfLevitation :public Potion{
public:
	PotionOfLevitation();
	virtual std::string getClassName() { return "PotionOfLevitation"; }
	static Item* Create(){ return new PotionOfLevitation(); }

	virtual String desc() {
		return
			String("Drinking this curious liquid will cause you to hover in the air, ") +
			String("able to drift effortlessly over traps. Flames and gases ") +
			String("fill the air, however, and cannot be bypassed while airborne.");
	}
	virtual int price() {
		return isKnown() ? 35 * quantity : Potion::price();
	}
protected:
	virtual void apply(Hero* hero);
};

class PotionOfMindVision :public Potion{
public:
	PotionOfMindVision();
	virtual std::string getClassName() { return "PotionOfMindVision"; }
	static Item* Create(){ return new PotionOfMindVision(); }

	virtual String desc() {
		return
			String("After drinking this, your mind will become attuned to the psychic signature ") +
			String("of distant creatures, enabling you to sense biological presences through walls. ") +
			String("Also this potion will permit you to see through nearby walls and doors.");
	}
	virtual int price() {
		return isKnown() ? 35 * quantity : Potion::price();
	}
protected:
	virtual void apply(Hero* hero);
};

class PotionOfPurity :public Potion{
private:
	static const String TXT_FRESHNESS;
	static const String TXT_NO_SMELL;

	static const int DISTANCE = 2;
public:
	PotionOfPurity();
	virtual std::string getClassName() { return "PotionOfPurity"; }
	static Item* Create(){ return new PotionOfPurity(); }

	virtual void shatter(int cell);
	virtual String desc() {
		return
			String("This reagent will quickly neutralize all harmful gases in the area of effect. ") +
			String("Drinking it will give you a temporary immunity to such gases.");
	}
	virtual int price() {
		return isKnown() ? 50 * quantity : Potion::price();
	}
protected:
	virtual void apply(Hero* hero);
};

class PotionOfInvisibility :public Potion{
private:
	static const float ALPHA;
public:
	PotionOfInvisibility();
	virtual std::string getClassName() { return "PotionOfInvisibility"; }
	static Item* Create(){ return new PotionOfInvisibility(); }

	virtual String desc() {
		return
			String("Drinking this potion will render you temporarily invisible. While invisible, ") +
			String("enemies will be unable to see you. Attacking an enemy, as well as using a wand or a scroll ") +
			String("before enemy's eyes, will dispel the effect.");
	}
	virtual int price() {
		return isKnown() ? 40 * quantity : Potion::price();
	}
	static void melt(Char* ch);
protected:
	virtual void apply(Hero* hero);
};

class PotionOfMight :public PotionOfStrength{
public:
	PotionOfMight();

	virtual std::string getClassName() { return "PotionOfMight"; }
	static Item* Create(){ return new PotionOfMight(); }

	virtual String desc() {
		return
			String("This powerful liquid will course through your muscles, permanently ") +
			String("increasing your strength by one point and health by five points.");
	}
	virtual int price() {
		return isKnown() ? 200 * quantity : PotionOfStrength::price();
	}
protected:
	virtual void apply(Hero* hero);
};

class PotionOfFrost :public Potion{
private:
	static const int DISTANCE = 2;
public:
	PotionOfFrost();

	virtual std::string getClassName() { return "PotionOfFrost"; }
	static Item* Create(){ return new PotionOfFrost(); }

	virtual void shatter(int cell);
	virtual String desc() {
		return
			String("Upon exposure to open air, this chemical will evaporate into a freezing cloud, causing ") +
			String("any creature that contacts it to be frozen in place, unable to act and move.");
	}
	virtual int price() {
		return isKnown() ? 50 * quantity : Potion::price();
	}
};