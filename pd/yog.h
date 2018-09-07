#pragma once

#include "mob.h"

class Yog :public Mob{
public:
	class RottingFist :public Mob{
	public:
		virtual CharSprite* Sprite();
		virtual std::string getClassName(){ return "RottingFist"; }
		static Mob* CreateRottingFist();

		RottingFist();

		virtual void die(const std::string& cause);
		virtual int attackSkill(Char* target) { return 36; }
		virtual int damageRoll();
		virtual int dr() { return 15; }
		virtual int attackProc(Char* enemy, int damage);
		virtual bool act();
		virtual std::string description() { return TXT_DESC; }
	private:
		static const int REGENERATION = 4;
	};

	class BurningFist :public Mob, public Object{
	public:
		virtual CharSprite* Sprite();
		virtual std::string getClassName(){ return "BurningFist"; }
		CLASSOBJECT(BurningFist);
		static Mob* CreateBurningFist();

		BurningFist();

		virtual void die(const std::string& cause);
		virtual int attackSkill(Char* target) { return 36; }
		virtual int damageRoll();
		virtual int dr() { return 15; }
		virtual bool attack(Char* enemy);
		virtual bool act();
		virtual std::string description() { return TXT_DESC; }
	protected:
		virtual bool canAttack(Char* enemy);
	};

	class Larva :public Mob{
	public:
		virtual CharSprite* Sprite();
		virtual std::string getClassName(){ return "Larva"; }
		static Mob* CreateLarva();

		Larva();

		virtual int attackSkill(Char* target) { return 30; }
		virtual int damageRoll();
		virtual int dr() { return 8; }
		virtual std::string description() { return TXT_DESC;  }
	};
public:
	virtual CharSprite* Sprite();
	virtual std::string getClassName(){ return "Yog"; }
	static Mob* CreateYog();

	Yog();
	void spawnFists();

	virtual void damage(int dmg, const std::string& src);
	virtual int defenseProc(Char* enemy, int damage);
	virtual void beckon(int cell) { }
	virtual void die(const std::string& cause);
	virtual void notice();
	virtual std::string description() { return TXT_DESC; }
private:
	static const std::string TXT_DESC;
	static int fistsCount;
};