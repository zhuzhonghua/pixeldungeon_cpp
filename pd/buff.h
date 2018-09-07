#pragma once

#include "actor.h"
#include "typedefine.h"
#include "bpt.h"
#include "herodoom.h"

class FlavourBuff;

class Buff :public Actor{
public:
	Buff();
	Char* target;

	virtual bool attachTo(Char* target);
	virtual void detach();

	virtual bool act() {
		diactivate();
		return true;
	}

	virtual int icon();

	static Buff* append(Char* target, const std::string& buff);
	static FlavourBuff* append(Char* target, const std::string& buffClass, float duration);
	static Buff* affect(Char* target, const std::string& buffClass);
	static FlavourBuff* affect(Char* target, const std::string& buffClass, float duration);
	static FlavourBuff* prolong(Char* target, const std::string& buffClass, float duration);

	static void detach(Buff* buff);
	static void detach(Char* target, const std::string& cl);

	virtual std::string toString() = 0;
};

FACTORY(Buff);

class FlavourBuff :public Buff{
public:
	virtual bool act() { detach(); return true; }
	virtual std::string getClassName(){ return "FlavourBuff"; }
	virtual std::string toString(){ return ""; };
};

class Light :public FlavourBuff{
public:
	CLASSNAME(Light);

	static const float DURATION;
	static const int DISTANCE = 4;

	virtual boolean attachTo(Char* target);
	virtual void detach();

	virtual int icon();

	virtual String toString() {
		return BPT::getText("lang.light");
	}
};

class BuffParalysis :public FlavourBuff{
private:
	static const float DURATION;
public:
	CLASSNAME(BuffParalysis);

	virtual boolean attachTo(Char* target);
	virtual void detach();
	virtual int icon();
	virtual String toString() {
		return BPT::getText("lang.buffparalysis");
	}
	static float duration(Char* ch);
	static void unfreeze(Char* ch);
};

class Frost :public FlavourBuff{
private:
	static const float DURATION;

public:
	CLASSNAME(Frost);

	virtual boolean attachTo(Char* target);
	virtual void detach();

	virtual int icon();
	virtual String toString() {
		return BPT::getText("lang.frost");
	}
	static float duration(Char* ch);
};

class BuffSlow :public FlavourBuff{
private:
	static const float DURATION;

public:
	CLASSNAME(BuffSlow);

	virtual int icon();
	virtual String toString() {
		return BPT::getText("lang.buffslow_str");
	}
	static float duration(Char* ch);
};

class Vertigo :public FlavourBuff{
public:
	static const float DURATION;

	CLASSNAME(Vertigo);

	virtual int icon();
	virtual String toString() {
		return BPT::getText("lang.vertigo_str");
	}
	static float duration(Char* ch);
};

class Terror :public FlavourBuff{
private:
	static const String OBJECT;
public:
	static const float DURATION;

	int object;
	
	Terror();
	CLASSNAME(Terror);

	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);

	virtual int icon();
	virtual String toString() {
		return BPT::getText("lang.terror_str");
	}
	static void recover(Char* target);
};

class Cripple :public FlavourBuff{
public:
	CLASSNAME(Cripple);

	static const float DURATION;

	virtual int icon();
	virtual String toString() {
		return BPT::getText("lang.crippled_str");;
	}
};

class Bleeding :public Buff, public Object{
protected:
	int level;

private:
	static const String LEVEL;

public:
	CLASSNAME(Bleeding);
	CLASSOBJECT(Bleeding);

	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);

	void set(int level) {
		this->level = level;
	};
	virtual int icon();
	virtual String toString() {
		return BPT::getText("lang.Bleeding");
	}
	virtual boolean act();
};

class Amok :public FlavourBuff{
public:
	CLASSNAME(Amok);
	virtual int icon();
	virtual String toString() {
		return BPT::getText("lang.Amok");
	}
};

class Sleep :public FlavourBuff{
public:
	static const float SWS;
	CLASSNAME(Sleep);
	virtual String toString() {
		return "Sleep";
	}
};

class Ooze :public Buff, public Object{
private:
	static const String TXT_HERO_KILLED;

public:
	int damage;

	Ooze();

	virtual int icon();
	virtual String toString() {
		return BPT::getText("lang.Caustic_ooze");
	}
	virtual boolean act();
	CLASSOBJECT(Ooze);
	CLASSNAME(Ooze);
};

class Levitation :public FlavourBuff{
public:
	static const float DURATION;
	virtual boolean attachTo(Char* target);
	virtual void detach();
	virtual int icon();
	virtual String toString() {
		return BPT::getText("lang.Levitating");// "Levitating";
	}
};

class Invisibility :public FlavourBuff{
public:
	static const float DURATION;

	virtual boolean attachTo(Char* target);
	virtual void detach();
	virtual int icon();
	virtual String toString() {
		return BPT::getText("lang.Invisible");// "Invisible";
	}
	CLASSNAME(Invisibility);
	static void dispel();
};

class Shadows :public Invisibility{
protected:
	float left;

private:
	static const String LEFT;

public:
	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);

	virtual boolean attachTo(Char* target);
	virtual void detach();
	virtual boolean act();
	void prolong() {
		left = 2;
	}
	virtual int icon();
	virtual String toString() {
		return BPT::getText("lang.Shadowmelded");// "Shadowmelded";
	}
	CLASSNAME(Shadows);
};

class Roots :public FlavourBuff{
public:
	CLASSNAME(Roots);
	virtual boolean attachTo(Char* target);
	virtual void detach();
	virtual int icon();
	virtual String toString() {
		return BPT::getText("lang.Rooted");// "Rooted";
	}
};

class Speed :public FlavourBuff{
public:
	static const float DURATION;
	CLASSNAME(Speed);
	virtual String toString() {
		return "";
	}
};

class MindVision :public FlavourBuff{
public:
	static const float DURATION;
	int distance;
	CLASSNAME(MindVision);

	MindVision();
	virtual int icon();
	virtual String toString() {
		return BPT::getText("lang.Mind_vision");// "Mind vision";
	}
	virtual void detach();
};

class Charm :public FlavourBuff{
private:
	static const String OBJECT;
public:
	int object;
	Charm();
	CLASSNAME(Charm);

	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);

	virtual int icon();
	virtual String toString() {
		return BPT::getText("lang.Charmed");// "Charmed";
	}
	static float durationFactor(Char* ch);
};

class Barkskin :public Buff{
private:
	int level;

public:
	Barkskin();
	virtual boolean act();
	int Level() {
		return level;
	}

	void Level(int value) {
		if (level < value) {
			level = value;
		}
	}
	virtual int icon();
	virtual String toString() {
		return BPT::getText("lang.Barkskin");// "Barkskin";
	}
	CLASSNAME(Barkskin);
};

class Hunger :public Buff ,public HeroDoom, public Object{
private:
	static const float STEP;
	static const String TXT_HUNGRY;
	static const String TXT_STARVING;
	static const String TXT_DEATH ;

	float level;

	static const String LEVEL;
public:
	static const float HUNGRY;
	static const float STARVING;

	CLASSOBJECT(Hunger);
	CLASSNAME(Hunger);

	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);
	virtual boolean act();

	void satisfy(float energy);
	boolean isStarving() {
		return level >= STARVING;
	}
	virtual int icon();
	virtual String toString(){
		if (level < STARVING) {
			return BPT::getText("lang.Hunger_toString_Hungry");// "Hungry";
		}
		else {
			return BPT::getText("lang.Hunger_toString_Starving");//"Starving";
		}
	}
	virtual void onDeath();
};

class Regeneration :public Buff{
private:
	static const float REGENERATION_DELAY;
	
public:
	virtual boolean act();
	CLASSNAME(Regeneration);

	virtual String toString(){ return "Regeneration"; }
};