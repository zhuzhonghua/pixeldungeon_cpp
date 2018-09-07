#pragma once

#include "actor.h"
#include "typedefine.h"

class Buff;
class CharSprite;

class Char :public Actor, public Object{
protected:
	static const std::string TXT_HIT;
	static const std::string TXT_KILL;
	static const std::string TXT_DEFEAT;

private:
	static const std::string TXT_YOU_MISSED;
	static const std::string TXT_SMB_MISSED;

	static const std::string TXT_OUT_OF_PARALYSIS;

	static const std::string POS;
	static const std::string TAG_HP;
	static const std::string TAG_HT;
	static const std::string BUFFS;

	static HashSet<std::string> EMPTY;

public:
	int pos;
	std::string name;

	int HT;
	int HP;
	CharSprite* sprite;
protected:
	float baseSpeed;

	virtual bool act();
	virtual void spend(float time);

	virtual void onRemove();
public:
	bool paralysed;
	bool rooted;
	bool flying;
	int invisible;

	int viewDistance;

	std::set<Buff*> buffs;

	Char();
	CLASSOBJECT(Char);

	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);

	HashSet<std::string>& resistances();
	HashSet<std::string>& immunities();
	bool immunitiesContain(const std::string& cls);

	void add(Buff* buff);
	void remove(Buff* buff);
	void remove(const std::string& buffClass);

	bool attack(Char* enemy);
	
	static bool hit(Char* attacker, Char* defender, bool magic);
	
	virtual int attackSkill(Char* target) { return 0; }
	virtual int defenseSkill(Char* enemy) { return 0; }

	virtual std::string defenseVerb() { return "dodged"; }
	virtual int dr() { return 0; }
	virtual int damageRoll() { return 1; }
	virtual int attackProc(Char* enemy, int damage) { return damage; }
	virtual int defenseProc(Char* enemy, int damage) { return damage; }

	float speed();
	virtual void damage(int dmg, const std::string& src);
	void destroy();
	virtual void die(const std::string& src);
	bool isAlive() { return HP > 0; }

	void Buffs(const std::string& c, std::set<Buff*>& re);
	Buff* buff(const std::string& c);
	std::set<Buff*> buffSet(const std::string& c);

	bool isCharmedBy(Char* ch);

	void updateSpriteState();

	virtual int stealth() { return 0; }
	virtual void move(int step);
	int distance(Char* other);

	void onMotionComplete() { next(); }

	void onAttackComplete() { next(); }

	void onOperateComplete() { next(); }
};