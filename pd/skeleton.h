#pragma once

#include "mob.h"

class Skeleton :public Mob{
public:
	Skeleton();

	static Mob* CreateSkeleton();
	virtual std::string getClassName() { return "Skeleton"; }
	virtual CharSprite* Sprite();

	virtual int damageRoll();
	virtual void die(const std::string& src);

private:
	static const std::string TXT_HERO_KILLED;
};