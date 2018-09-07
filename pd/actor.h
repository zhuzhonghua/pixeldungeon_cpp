#pragma once

#include "bundlable.h"
#include "typedefine.h"
#include <limits>
#include <set>
#include <map>
#include <vector>

class Char;

class Actor :public Bundlable{
public:
	static const int TICK = 1;

	Actor();
	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);

	int Id();

	static void fixTime();
	static void clear();
	static Char* findChar(int pos);
	static Actor* findById(int id);
	static void init();
	static void addDelayed(Actor* actor, float delay);
	static void remove(Actor* actor);
	static void add(Actor* actor);
	
	static void occupyCell(Char* ch);
	static void freeCell(int pos);
	void next() { if (current == this) current = NULL; }
	static void process();
	
	static HashSet<Actor*> all;
private:
	float time;

	int id;

	static const std::string TIME;
	static const std::string ID;
	static float now;

	
	static Actor* current;
	static std::map<int, Actor*> ids;
	static std::vector<Char*> chars;

	
	static void add(Actor* actor, float time);
protected:
	virtual bool act() = 0;

	virtual void spend(float time) { this->time += time; }
	void postpone(float time);
	float cooldown() { return time - now; }
	void diactivate() { time = std::numeric_limits<float>::max(); }

	virtual void onAdd(){}
	virtual void onRemove() {}

};