#pragma once

#include "bundlable.h"
#include "item.h"
#include "buff.h"

class PlantSprite;
class Char;
class Plant;

typedef Plant* (*CreatePlant)();

class Plant :public Bundlable{
public:
	static Plant* Create() { return new Plant(); }
	class Seed :public Item{
	public:
		static const std::string AC_PLANT;
		std::string alchemyClass;

		Seed();
		virtual void actions(Hero* hero, std::vector<std::string>& actions);
		virtual void execute(Hero* hero, std::string action);

		Plant* couch(int pos);

		virtual bool isUpgradable() { return false; }
		virtual bool isIdentified() { return true; }
		virtual int price() { return 10 * quantity; }
		virtual std::string info();
	protected:
		virtual void onThrow(int cell);

		CreatePlant plantClass;
		std::string plantName;

	private:
		static const std::string TXT_INFO;
		static const float TIME_TO_PLANT;
	};
public:
	std::string plantName;

	int image;
	int pos;

	PlantSprite* sprite;

	virtual void activate(Char* ch);
	void wither();

	virtual void restoreFromBundle(Bundle* bundle);
	virtual void storeInBundle(Bundle* bundle);
	virtual std::string getClassName() { return "Plant"; }

	virtual std::string desc() { return NULL; }
private:
	static const std::string POS;
};

class Firebloom :public Plant{
private:
	static const String TXT_DESC;
public:
	Firebloom();
	static Plant* Create() { return new Plant(); }

	virtual void activate(Char* ch);
	virtual String desc() {
		return TXT_DESC;
	}

	class Seed :public Plant::Seed{
	public:
		Seed();
		virtual std::string getClassName() { return "Firebloom::Seed"; }
		static Item* Create() { return new Seed(); }

		virtual String desc() {
			return TXT_DESC;
		}
	};
};

class Icecap :public Plant{
private:
	static const String TXT_DESC;
public:
	Icecap();
	static Plant* Create() { return new Icecap(); }
	virtual void activate(Char* ch);

	virtual String desc() {
		return TXT_DESC;
	}

	class Seed :public Plant::Seed{
	public:
		Seed();
		virtual std::string getClassName() { return "Icecap::Seed"; }
		static Item* Create() { return new Seed(); }

		virtual String desc() {
			return TXT_DESC;
		}
	};
};

class Sorrowmoss :public Plant{
private:
	static const String TXT_DESC;

public:
	Sorrowmoss();
	static Plant* Create() { return new Sorrowmoss(); }
	virtual void activate(Char* ch);
	virtual String desc() {
		return TXT_DESC;
	}

	class Seed :public Plant::Seed{
	public:
		Seed();
		virtual std::string getClassName() { return "Sorrowmoss::Seed"; }
		static Item* Create() { return new Seed(); }

		virtual String desc() {
			return TXT_DESC;
		}
	};
};

class Dreamweed :public Plant{
private:
	static const String TXT_DESC;

public:
	Dreamweed();
	static Plant* Create() { return new Dreamweed(); }
	virtual void activate(Char* ch);

	virtual String desc() {
		return TXT_DESC;
	}

	class Seed :public Plant::Seed{
	public:
		Seed();
		virtual std::string getClassName() { return "Dreamweed::Seed"; }
		static Item* Create() { return new Seed(); }

		virtual String desc() {
			return TXT_DESC;
		}
	};
};


class Sungrass :public Plant{
private:
	static const String TXT_DESC;

public:
	Sungrass();
	static Plant* Create() { return new Sungrass(); }
	virtual void activate(Char* ch);

	virtual String desc() {
		return TXT_DESC;
	}

	class Seed :public Plant::Seed{
	public:
		Seed();
		virtual std::string getClassName() { return "Sungrass::Seed"; }
		static Item* Create() { return new Seed(); }

		virtual String desc() {
			return TXT_DESC;
		}
	};

	class Health :public Buff{
	private:
		static const float STEP;

		int pos;
		static const String POS;
	public:
		virtual bool attachTo(Char* target);
		virtual boolean act();
		virtual int icon();
		virtual String toString() {
			return BPT::getText("lang.Sungrass_Health_toString");// "Herbal healing";
		}

		virtual std::string getClassName() { return "Health"; }
		virtual void storeInBundle(Bundle* bundle);
		virtual void restoreFromBundle(Bundle* bundle);
	};
};

class Earthroot :public Plant{
private:
	static const String TXT_DESC ;

public:
	Earthroot();
	static Plant* Create() { return new Earthroot(); }
	virtual void activate(Char* ch);

	virtual String desc() {
		return TXT_DESC;
	}

	class Seed :public Plant::Seed{
	public:
		Seed();
		virtual std::string getClassName() { return "Earthroot::Seed"; }
		static Item* Create() { return new Seed(); }

		virtual String desc() {
			return TXT_DESC;
		}
	};

	class Armor :public Buff{
	private:
		static const float STEP;

		int pos;
		
		static const String POS;
		static const String LEVEL;
	public:
		int level;
		virtual bool attachTo(Char* target);
		virtual boolean act();

		int absorb(int damage);
		void Level(int value) {
			if (level < value) {
				level = value;
			}
		}
		virtual int icon();

		virtual String toString() {
			return "Herbal armor";
		}
		virtual std::string getClassName() { return "Armor"; }
		virtual void storeInBundle(Bundle* bundle);
		virtual void restoreFromBundle(Bundle* bundle);
	};
};

class Fadeleaf :public Plant{
private:
	static const String TXT_DESC;

public:
	Fadeleaf();

	static Plant* Create() { return new Fadeleaf(); }
	virtual void activate(Char* ch);

	virtual String desc() {
		return TXT_DESC;
	}

	class Seed :public Plant::Seed{
	public:
		Seed();
		virtual std::string getClassName() { return "Fadeleaf::Seed"; }
		static Item* Create() { return new Seed(); }

		virtual String desc() {
			return TXT_DESC;
		}
	};
};

class Rotberry :public Plant{
private:
	static const String TXT_DESC ;

public:
	Rotberry();

	static Plant* Create() { return new Fadeleaf(); }
	virtual void activate(Char* ch);

	virtual String desc() {
		return TXT_DESC;
	}

	class Seed :public Plant::Seed{
	public:
		Seed();
		virtual std::string getClassName() { return "Rotberry::Seed"; }
		static Item* Create() { return new Seed(); }

		virtual boolean collect(Bag* container);
		virtual String desc() {
			return TXT_DESC;
		}
	};
};