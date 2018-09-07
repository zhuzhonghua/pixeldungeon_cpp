#pragma once

#include "charsprite.h"
#include "heroclass.h"

class HeroSprite :public CharSprite{
private:
	static const int FRAME_WIDTH = 12;
	static const int FRAME_HEIGHT = 15;

	static const int RUN_FRAMERATE = 20;

	static TextureFilm* tiers;

	Animation* fly;
	Animation* read;

public:
	HeroSprite();
	void updateArmor();
	virtual void place(int p);
	virtual void move(int from, int to);
	virtual void jump(int from, int to, Callback* callback);
	void Idle() { play(idle); }
	void Read();
	virtual void update();
	bool sprint(bool on);
	static Image* avatar(HeroClass cl, int armorTier);

	static TextureFilm* Tiers();
};