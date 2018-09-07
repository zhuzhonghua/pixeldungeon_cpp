#pragma once

#include "item.h"
#include "typedefine.h"
#include "bpt.h"

class Char;

class Bag:public Item{
public:
	static const std::string AC_OPEN;
	Char* owner;
	ArrayList<Item*> items;
	int size;

	
	Bag();
	virtual bool grab(Item* item) { return false; }

	virtual std::string getClassName() { return "Bag"; }
	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);

	bool contains(Item* item);

public:
	ArrayList<Item*>::iterator itr;
	Bag* nextBag;
	void Begin();
	Item* Next();
	bool HasNext();
private:
	static const std::string ITEMS;
};


class SeedPouch :public Bag{
public:
	SeedPouch();

	CLASSNAME(SeedPouch);
	virtual boolean grab(Item* item);
	virtual int price() { return 50; }

	virtual String info() {
		return BPT::getText("lang.SeedPouch_Info");
			//"This small velvet pouch allows you to store any number of seeds in it. Very convenient.";
	}
};