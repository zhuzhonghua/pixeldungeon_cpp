#pragma once

#include "mob.h"

class King :public Mob{
public:
	class Undead :public Mob{
	public:
		static int count;

		virtual CharSprite* Sprite();
		virtual std::string getClassName() { return "KingUndead"; }
		static Mob* CreateUndead();

		Undead();
		virtual int damageRoll();
		virtual int attackSkill(Char* target) { return 16; }
		virtual int attackProc(Char* enemy, int damage);
		virtual void damage(int dmg, const std::string& src);
		virtual void die(const std::string& cause);
		virtual int dr() { return 5; }
		virtual std::string defenseVerb() { return "blocked"; }
		virtual std::string description(){
			return
				std::string("These undead dwarves, risen by the will of the King of Dwarves, were members of his court. ") +
				std::string("They appear as skeletons with a stunning amount of facial hair.");
		}
	protected:
		virtual void onAdd();
		virtual void onRemove();
	};
public:
	virtual CharSprite* Sprite();
	virtual std::string getClassName() { return "King"; }
	static Mob* CreateKing();

	King();
	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);

	virtual int damageRoll();
	virtual int attackSkill(Char* target) { return 32; }
	virtual int dr() { return 14; }
	virtual std::string defenseVerb() { return "parried"; }
	virtual bool attack(Char* enemy);
	virtual void die(const std::string& cause);
	virtual void notice();
	virtual std::string description() {
		return
			std::string("The last king of dwarves was known for his deep understanding of processes of life and death. ") +
			std::string("He has persuaded members of his court to participate in a ritual, that should have granted them ") +
			std::string("eternal youthfulness. In the end he was the only one, who got it - and an army of undead ") +
			std::string("as a bonus.");
	}
protected:
	virtual bool getCloser(int target);
	virtual bool canAttack(Char* enemy);
private:
	bool nextPedestal;
	static const int MAX_ARMY_SIZE = 5;
	static const std::string PEDESTAL;

	bool canTryToSummon();
	int maxArmySize();
	void summon();
};