#pragma once

#include "char.h"
#include "wndbag.h"
#include "bpt.h"

class Hero;

class Mob :public Char{
public:
	class AiState 
	{
	public:
		AiState(Mob* m) :owner(m){}
		Mob* owner;
		virtual bool act(bool enemyInFOV, bool justAlerted) = 0;
		virtual std::string status() = 0;
	};

	Mob();

	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);
	virtual CharSprite* Sprite() = 0;

	virtual void add(Buff* buff);
	virtual void remove(Buff* buff);
	virtual void move(int step);
	virtual void onAttackComplete();
	virtual int defenseSkill(Char* enemy);
	virtual int defenseProc(Char* enemy, int damage);
	void aggro(Char* ch);
	virtual void notice();
	virtual void beckon(int cell);
	virtual void damage(int dmg, const std::string& src);
	void yell(const std::string& str);
	virtual std::string description() { return BPT::getText("lang.Mob_description"); }
	virtual void destroy();

	int exp();

	virtual void die(const std::string& str);

	bool hostile;

	AiState* SLEEPEING;
	AiState* HUNTING;
	AiState* WANDERING;
	AiState* FLEEING;
	AiState* PASSIVE;
	AiState* state;
protected:
	static const String	TXT_ECHO;
	static const String TXT_NOTICE1;
	static const String TXT_RAGE;
	static const String TXT_EXP;

	int target;

	int _defenseSkill;

	int EXP;
	int maxLvl;

	Char* enemy;
	bool enemySeen;
	bool alerted;

	static const float TIME_TO_WAKE_UP;

	float lootChance;
	
	virtual bool act();
	Char* chooseEnemy();
	bool moveSprite(int from, int to);
	virtual bool canAttack(Char* enemy);
	virtual bool getCloser(int target);
	virtual bool getFurther(int target);
	virtual float attackDelay() { return 1.0f; }
	virtual bool doAttack(Char* enemy);

	class Fleeing :public AiState{
	public:
		Fleeing(Mob* m) :AiState(m){}

		static const std::string TAG;
		virtual bool act(bool enemyInFOV, bool justAlerted);
		virtual std::string status();
	protected:
		void nowhereToRun() {}
	};
private:
	static const String	TXT_DIED;

	static const std::string STATE;
	static const std::string TARGET;

	class Sleeping :public AiState{
	public:
		Sleeping(Mob* m) :AiState(m){}
		static const std::string TAG;

		virtual bool act(bool enemyInFOV, bool justAlerted);
		virtual std::string status();
	};
	class Wandering :public AiState{
	public:
		static const std::string TAG;
		Wandering(Mob* m) :AiState(m){}

		virtual bool act(bool enemyInFOV, bool justAlerted);
		virtual std::string status();
	};
	class Hunting :public AiState{
	public:
		static const std::string TAG;

		Hunting(Mob* m) :AiState(m){}

		virtual bool act(bool enemyInFOV, bool justAlerted);
		virtual std::string status();
	};
	class Passive :public AiState{
	public:
		static const std::string TAG;
		Passive(Mob* m) :AiState(m){}

		virtual bool act(bool enemyInFOV, bool justAlerted);
		virtual std::string status();
	};
};

FACTORY(Mob);

typedef Mob* (*CreateMob)();

class NPC :public Mob{
public:
	NPC();

	virtual void beckon(int cell) {	}
	virtual void interact() = 0;
protected:
	void throwItem();
};

class Shopkeeper :public NPC{
public:
	Shopkeeper();
	CLASSNAME(Shopkeeper);

	virtual CharSprite* Sprite();

	virtual void damage(int dmg, const std::string& src) {
		flee();
	}

	virtual void add(Buff* buff){ flee(); }

	virtual boolean reset() {
		return true;
	}
	virtual String description() {
		return BPT::getText("lang.Shopkeeper_description");
	}
	static WndBag* sell();

	virtual void interact(){ sell(); }
protected:
	virtual boolean act();

	void flee();

private:
	static WndBag::Listener* itemSelector;
};

class MirrorImage :public NPC{
public:
	int tier;

private:
	int attack;
	int damage;

	static const String TIER;
	static const String ATTACK;
	static const String DAMAGE;
public:
	virtual CharSprite* Sprite();
	MirrorImage();

	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);
	void duplicate(Hero* hero);
	virtual int attackSkill(Char* target) {
		return attack;
	}
	virtual int damageRoll() {
		return damage;
	}
	virtual int attackProc(Char* enemy, int damage);
	virtual String description() {
		return BPT::getText("lang.MirrorImage_description");
			//"This illusion bears a close resemblance to you, " +
			//"but it's paler and twitches a little.";
	}
	virtual void interact();
	CLASSNAME(MirrorImage);
protected:
	Char* chooseEnemy();
};