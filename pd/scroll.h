#pragma once

#include "item.h"
#include "typedefine.h"
#include "itemstatushandler.h"

class Scroll :public Item{
private:
	static const String TXT_BLINDED;
	static String _scrolls[];
	static std::vector<String> scrolls;

	static String _runes[];
	static std::vector<String> runes;

	static int _images[];
	static std::vector<int> images;

	static ItemStatusHandler* handler;

	String rune;
public:
	static const String AC_READ;

	Scroll();

	static void initLabels();
	static void save(Bundle* bundle);
	static void restore(Bundle* bundle);

	virtual void actions(Hero* hero, std::vector<std::string>& actions);
	virtual void execute(Hero* hero, const std::string action);

	virtual bool isKnown();
	void setKnown();

	virtual Item* identify();
	virtual std::string Name() {
		return isKnown() ? name : "scroll \"" + rune + "\"";
	}
	virtual String info();
	virtual bool isUpgradable() {
		return false;
	}
	virtual boolean isIdentified() {
		return isKnown();
	}
	static std::set<std::string> getKnown();
	static std::set<std::string> getUnknown() {
		return handler->unknown();
	}
	static boolean allKnown() {
		return handler->known.size() == scrolls.size();
	}
	virtual int price() {
		return 15 * quantity;
	}
protected:
	static const float TIME_TO_READ;

	virtual void doRead() = 0;

	void readAnimation();
};

class InventoryScroll :public Scroll{
protected:
	static boolean identifiedByUse;
	String inventoryTitle;
	//WndBag.Mode mode = WndBag.Mode.ALL;

	virtual void doRead();
	virtual void onItemSelected(Item* item) = 0;
private:
	static const String TXT_WARNING;
	static const String TXT_YES;
	static const String TXT_NO;

	void confirmCancelation();
public:
	InventoryScroll();
};

class ScrollOfIdentify :public InventoryScroll{
public:
	ScrollOfIdentify();

	virtual String desc() {
		return
			std::string("Permanently reveals all of the secrets of a single item.");
	}
	virtual int price() {
		return isKnown() ? 30 * quantity : InventoryScroll::price();
	}

	virtual std::string getClassName() { return "ScrollOfIdentify"; }
	static Item* Create(){ return new ScrollOfIdentify(); }
protected:
	virtual void onItemSelected(Item* item);
};

class ScrollOfMagicMapping :public Scroll{
public:
	ScrollOfMagicMapping();

	virtual String desc() {
		return
			std::string("When this scroll is read, an image of crystal clarity will be etched into your memory, ") +
			std::string("alerting you to the precise layout of the level and revealing all hidden secrets. ") +
			std::string("The locations of items and creatures will remain unknown.");
	}
	virtual int price() {
		return isKnown() ? 25 * quantity : Scroll::price();
	}
	static void discover(int cell);
	virtual std::string getClassName() { return "ScrollOfMagicMapping"; }
	static Item* Create(){ return new ScrollOfMagicMapping(); }
protected:
	virtual void doRead();
private:
	static const String TXT_LAYOUT;
};

class ScrollOfRecharging :public Scroll{
public:
	ScrollOfRecharging();

	virtual String desc() {
		return
			std::string("The raw magical power bound up in this parchment will, when released, ") +
			std::string("recharge all of the reader's wands to full power.");
	}

	static void charge(Hero* hero);
	virtual int price() {
		return isKnown() ? 40 * quantity : Scroll::price();
	}
	virtual std::string getClassName() { return "ScrollOfRecharging"; }
	static Item* Create(){ return new ScrollOfRecharging(); }
protected:
	virtual void doRead();
};

class ScrollOfRemoveCurse :public Scroll{
public:
	ScrollOfRemoveCurse();

	virtual String desc() {
		return
			std::string("The incantation on this scroll will instantly strip from ") +
			std::string("the reader's weapon, armor, rings and carried items any evil ") +
			std::string("enchantments that might prevent the wearer from removing them.");
	}

	static boolean uncurse(Hero* hero, const std::vector<Item*>& items);
	virtual int price() {
		return isKnown() ? 30 * quantity : Scroll::price();
	}
	virtual std::string getClassName() { return "ScrollOfRemoveCurse"; }
	static Item* Create(){ return new ScrollOfRemoveCurse(); }
protected:
	virtual void doRead();
private:
	static const String TXT_PROCCED;
	static const String TXT_NOT_PROCCED;
};

class ScrollOfTeleportation :public Scroll{
public:
	static const String TXT_TELEPORTED;

	static const String TXT_NO_TELEPORT;

	ScrollOfTeleportation();

	static void teleportHero(Hero*  hero);

	virtual String desc() {
		return
			std::string("The spell on this parchment instantly transports the reader ") +
			std::string("to a random location on the dungeon level. It can be used ") +
			std::string("to escape a dangerous situation, but the unlucky reader might ") +
			std::string("find himself in an even more dangerous place.");
	}
	virtual int price() {
		return isKnown() ? 40 * quantity : Scroll::price();
	}
	virtual std::string getClassName() { return "ScrollOfTeleportation"; }
	static Item* Create(){ return new ScrollOfTeleportation(); }
protected:
	virtual void doRead();
};

class ScrollOfChallenge :public Scroll{
public:
	ScrollOfChallenge();

	virtual String desc() {
		return
			std::string("When read aloud, this scroll will unleash a challenging roar ") +
			std::string("that will awaken all monsters and alert them to the reader's location.");
	}
	virtual std::string getClassName() { return "ScrollOfChallenge"; }
	static Item* Create(){ return new ScrollOfChallenge(); }
protected:
	virtual void doRead();
};

class ScrollOfTerror :public Scroll{
public:
	ScrollOfTerror();

	virtual String desc() {
		return
			std::string("A flash of red light will overwhelm all creatures in your field of view with terror, ") +
			std::string("and they will turn and flee. Attacking a fleeing enemy will dispel the effect.");
	}
	virtual int price() {
		return isKnown() ? 50 * quantity : Scroll::price();
	}
	virtual std::string getClassName() { return "ScrollOfTerror"; }
	static Item* Create(){ return new ScrollOfTerror(); }
protected:
	virtual void doRead();
};

class ScrollOfLullaby :public Scroll{
public:
	ScrollOfLullaby();

	virtual String desc() {
		return
			std::string("A soothing melody will put all creatures in your field of view into a deep sleep, ") +
			std::string("giving you a chance to flee or make a surprise attack on them.");
	}
	virtual int price() {
		return isKnown() ? 50 * quantity : Scroll::price();
	}
	virtual std::string getClassName() { return "ScrollOfLullaby"; }
	static Item* Create(){ return new ScrollOfLullaby(); }
protected:
	virtual void doRead();
};

class ScrollOfPsionicBlast :public Scroll{
public:
	ScrollOfPsionicBlast();

	virtual String desc() {
		return
			std::string("This scroll contains destructive energy, that can be psionically channeled to inflict a ") +
			std::string("massive damage to all creatures within a field of view. An accompanying flash of light will ") +
			std::string("temporarily blind everybody in the area of effect including the reader of the scroll.");
	}
	virtual int price() {
		return isKnown() ? 80 * quantity : Scroll::price();
	}
	virtual std::string getClassName() { return "ScrollOfPsionicBlast"; }
	static Item* Create(){ return new ScrollOfPsionicBlast(); }
protected:
	virtual void doRead();
};

class ScrollOfMirrorImage :public Scroll{
private:
	static const int NIMAGES = 3;
public:
	ScrollOfMirrorImage();
	virtual String desc() {
		return
			"The incantation on this scroll will create illusionary twins of the reader, which will chase his enemies.";
	}
	virtual std::string getClassName() { return "ScrollOfMirrorImage"; }
	static Item* Create(){ return new ScrollOfMirrorImage(); }
protected:
	virtual void doRead();
};

class ScrollOfUpgrade :public InventoryScroll{
private:
	static const String TXT_LOOKS_BETTER;
public:
	ScrollOfUpgrade();

	static void upgrade(Hero* hero);

	virtual String desc() {
		return
			std::string("This scroll will upgrade a single item, improving its quality. A wand will ") +
			std::string("increase in power and in number of charges; a weapon will inflict more damage ") +
			std::string("or find its mark more frequently; a suit of armor will deflect additional blows; ") +
			std::string("the effect of a ring on its wearer will intensify. Weapons and armor will also ") +
			std::string("require less strength to use, and any curses on the item will be lifted.");
	}
	virtual std::string getClassName() { return "ScrollOfUpgrade"; }
	static Item* Create(){ return new ScrollOfUpgrade(); }
protected:
	virtual void onItemSelected(Item* item);
};

class ScrollOfEnchantment :public InventoryScroll{
private:
	static const String TXT_GLOWS;

protected:
	virtual void onItemSelected(Item* item);
public:
	ScrollOfEnchantment();

	virtual String desc() {
		return
			std::string("This scroll is able to imbue a weapon or an armor ") +
			std::string("with a random enchantment, granting it a special power.");
	}
	virtual std::string getClassName() { return "ScrollOfEnchantment"; }
	static Item* Create(){ return new ScrollOfEnchantment(); }
};