#include "actor.h"
#include "bundle.h"
#include "char.h"
#include "level.h"
#include "dungeon.h"
#include "statistics.h"
#include "buff.h"
#include "charsprite.h"
#include "mob.h"
#include "blob.h"
#include "hero.h"

const std::string Actor::TIME = "time";
const std::string Actor::ID = "id";

float Actor::now = 0;

void Actor::process()
{
	if (current != NULL) 
	{
		return;
	}

	bool doNext=false;

	do {
		now = std::numeric_limits<float>::max();
		current = NULL;

		for (int i = 0; i <chars.size();i++)
		{
			chars[i] = NULL;
		}
		//Arrays.fill(chars, null);

		for (std::set<Actor*>::iterator itr = all.begin();
			itr != all.end(); itr++)
		{
			Actor* actor = *itr;
			if (actor->time < now) 
			{
				now = actor->time;
				current = actor;
			}

			if (dynamic_cast<Char*>(actor)) 
			{
				Char* ch = (Char*)actor;
				chars[ch->pos] = ch;
			}
		}

		if (current != NULL) {

			if (dynamic_cast<Char*>(current) != NULL && ((Char*)current)->sprite->isMoving) 
			{
				// If it's character's turn to act, but its sprite 
				// is moving, wait till the movement is over
				current = NULL;
				break;
			}

			doNext = current->act();
			if (doNext && !Dungeon::hero->isAlive()) 
			{
				doNext = false;
				current = NULL;
			}
		}
		else 
		{
			doNext = false;
		}

	} while (doNext);
}

HashSet<Actor*> Actor::all;
Actor* Actor::current;
std::map<int, Actor*> Actor::ids;
std::vector<Char*> Actor::chars(Level::LENGTH);

Actor::Actor()
{
	id = 0;
	time = 0;
}

void Actor::postpone(float time)
{
	if (this->time < now + time) {
		this->time = now + time;
	}
}

void Actor::storeInBundle(Bundle* bundle)
{
	bundle->put(TIME, time);
	bundle->put(ID, id);
}

void Actor::restoreFromBundle(Bundle* bundle)
{
	time = bundle->getFloat(TIME);
	id = bundle->getInt(ID);
}

int Actor::Id()
{
	if (id > 0) 
	{
		return id;
	}
	else 
	{
		int max = 0;
		for (std::set<Actor*>::iterator itr = all.begin();
			itr != all.end(); itr++)
		{
			Actor* a = *itr;
			if (a->id > max) 
			{
				max = a->id;
			}
		}
		return (id = max + 1);
	}
}

void Actor::fixTime()
{
	if (Dungeon::hero != NULL && all.find(Dungeon::hero) != all.end()) 
	{
		Statistics::duration += now;
	}

	float min = std::numeric_limits<float>::max();
	for (std::set<Actor*>::iterator itr = all.begin();
		itr != all.end(); itr++)
	{
		Actor* a = *itr;
		if (a->time < min) 
		{
			min = a->time;
		}
	}
	for (std::set<Actor*>::iterator itr = all.begin();
		itr != all.end(); itr++)
	{
		Actor* a = *itr;
		a->time -= min;
	}
	now = 0;
}

void Actor::clear()
{
	now = 0;

	for (int i = 0; i < chars.size(); i++)
	{
		chars[i] = NULL;
	}
	
	all.clear();

	ids.clear();
}

Char* Actor::findChar(int pos)
{
	return chars[pos];
}

Actor* Actor::findById(int id)
{
	std::map<int, Actor*>::iterator itr = ids.find(id);
	if (itr != ids.end()) return itr->second;
	return NULL;
}

void Actor::init()
{
	addDelayed(Dungeon::hero, -std::numeric_limits<float>::min());

	for (std::set<Mob*>::iterator itr = Dungeon::level->mobs.begin();
		itr != Dungeon::level->mobs.end(); itr++)
	{
		Actor::add(*itr);
	}

	for (std::map<std::string, Blob*>::iterator itr = Dungeon::level->blobs.begin();
		itr != Dungeon::level->blobs.end(); itr++)
	{
		Blob* blob = itr->second;
		Actor::add(blob);
	}

	current = NULL;
}

void Actor::addDelayed(Actor* actor, float delay)
{
	add(actor, now + delay);
}

void Actor::add(Actor* actor)
{
	add(actor, now);
}

void Actor::add(Actor* actor, float time)
{
	if (all.find(actor) != all.end()) 
	{
		return;
	}

	if (actor->id > 0) 
	{
		ids.insert(std::make_pair(actor->id, actor));
	}

	all.insert(actor);
	actor->time += time;
	actor->onAdd();

	if (dynamic_cast<Char*>(actor)) 
	{
		Char* ch = (Char*)actor;
		chars[ch->pos] = ch;
		for (std::set<Buff*>::iterator itr = ch->buffs.begin();
			itr != ch->buffs.end(); itr++)
		{
			Buff* buf = *itr;
			all.insert(buf);
			buf->onAdd();
		}
	}
}

void Actor::occupyCell(Char* ch)
{
	chars[ch->pos] = ch;
}

void Actor::freeCell(int pos)
{
	chars[pos] = NULL;
}

void Actor::remove(Actor* actor)
{
	if (actor != NULL) 
	{
		std::set<Actor*>::iterator itr = all.find(actor);
		if (itr != all.end()) all.erase(itr);

		actor->onRemove();

		if (actor->id > 0) 
		{
			std::map<int, Actor*>::iterator itr2 = ids.find(actor->id);
			if (itr2 != ids.end()) ids.erase(itr2);
		}
	}
}