#pragma once

#include "charsprite.h"
#include "pixelparticle.h"

class MobSprite :public CharSprite{
private:
	static const float FADE_TIME;
	static const float FALL_TIME;

public:
	virtual void update();
	virtual void onComplete(Animation* anim);

	void fall();
};

class RatSprite :public MobSprite{
public:
	RatSprite();
};

class GnollSprite :public MobSprite{
public:
	GnollSprite();
};

class CrabSprite :public MobSprite{
public:
	CrabSprite();

	virtual int blood() { return 0xFFFFEA80; }
};

class SwarmSprite :public MobSprite{
public:
	SwarmSprite();

	virtual int blood() { return 0xFF8BA077; }
};

class SkeletonSprite :public MobSprite{
public:
	SkeletonSprite();

	virtual void Die();
	virtual int blood() { return 0xFFcccccc; }
};

class ThiefSprite :public MobSprite{
public:
	ThiefSprite();
};

class GooSprite :public MobSprite{
public:
	class GooParticle :public Shrinking{
	public:
		static Emitter::Factory* FACTORY;

		GooParticle();

		void reset(float x, float y);
		virtual void update();
	};
public:
	GooSprite();

	void pumpUp() { MobSprite::play(pump); }
	virtual void play(Animation* anim, bool force);
	virtual int blood() { return 0xFF000000; }
private:
	Animation* pump;
	Animation* jump;

	Emitter* spray;
};

class ShamanSprite :public MobSprite{
public:
	ShamanSprite();
	void Zap(int pos);
private:
	int points[2];
};

class BatSprite :public MobSprite{
public:
	BatSprite();
};

class BruteSprite :public MobSprite{
public:
	BruteSprite();
};

class TenguSprite :public MobSprite{
public:
	TenguSprite();

	virtual void move(int from, int to);
	virtual void Attack(int cell);
	virtual void onComplete(Animation* anim);
private:
	Animation* cast;
};

class SpinnerSprite :public MobSprite{
public:
	SpinnerSprite();

	virtual int blood() { return 0xFFBFE5B8; }
};

class ElementalSprite :public MobSprite{
public:
	ElementalSprite();

	virtual int blood() { return 0xFFFF7D13; }
	virtual void link(Char* ch)
	{
		MobSprite::link(ch);
		add(State::BURNING);
	}
	virtual void Die()
	{
		MobSprite::Die();
	}
};

class MonkSprite :public MobSprite{
private:
	Animation* kick;
public:
	MonkSprite();

	virtual void Attack(int cell);
	virtual void onComplete(Animation* anim);
};

class DM300Sprite :public MobSprite{
public:
	DM300Sprite();

	virtual void onComplete(Animation* anim);
	virtual int blood() { return 0xFFFFFF88; }
};

class WarlockSprite :public MobSprite{
public:
	WarlockSprite();
	void Zap(int cell);
	virtual void onComplete(Animation* anim);
};

class GolemSprite :public MobSprite{
public:
	GolemSprite();
	virtual int blood() { return 0xFF80706c; }
	virtual void onComplete(Animation* anim);
};

class SuccubusSprite :public MobSprite{
public:
	SuccubusSprite();

	virtual void Die();
};

class KingSprite :public MobSprite{
public:
	KingSprite();
};

class UndeadSprite :public MobSprite{
public:
	UndeadSprite();

	virtual void Die();
	virtual int blood() { return 0xFFcccccc; }
};

class EyeSprite :public MobSprite{
public:
	EyeSprite();
	virtual void Attack(int pos);
	virtual void onComplete(Animation* anim);
private:
	int attackPos;
};

class ScorpioSprite :public MobSprite{
public:
	ScorpioSprite();
	virtual int blood() { return 0xFF44FF22; }
	virtual void Attack(int cell);
	virtual void onComplete(Animation* anim);
private:
	int cellToAttack;
};


class YogSprite :public MobSprite{
public:
	YogSprite();
	virtual void Die();
};

class RottingFistSprite :public MobSprite{
public:
	RottingFistSprite();

	virtual void Attack(int cell);
	virtual void onComplete(Animation* anim);
private:
	static const float FALL_SPEED;
};

class BurningFistSprite :public MobSprite{
public:
	BurningFistSprite();

	virtual void Attack(int cell);
	virtual void onComplete(Animation* anim);
private:
	int posToShoot;
};

class LarvaSprite :public MobSprite{
public:
	LarvaSprite();

	virtual int blood() { return 0xbbcc66; }
	virtual void Die();
};

class SheepSprite :public MobSprite{
public:
	SheepSprite();
};

class MimicSprite :public MobSprite{
public:
	MimicSprite();
	virtual int blood() { return 0xFFcb9700; }

};

class WraithSprite :public MobSprite{
public:
	WraithSprite();
	virtual int blood() { return 0x88000000; }
};

class ShopkeeperSprite :public MobSprite{
private:
	PixelParticle* coin;

public:
	ShopkeeperSprite();
	virtual void onComplete(Animation* anim);
};

class MirrorSprite :public MobSprite{
private:
	static const int FRAME_WIDTH = 12;
	static const int FRAME_HEIGHT = 15;

public:
	MirrorSprite();
	virtual void link(Char* ch);
	void updateArmor(int tier);
};