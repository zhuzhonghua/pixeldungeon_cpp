#pragma once

#include "component.h"

#include <map>

class SmartTexture;
class TextureFilm;
class Char;
class Image;

class BuffIndicator :public Component{
public:
	static const int NONE = -1;

	static const int MIND_VISION = 0;
	static const int LEVITATION = 1;
	static const int FIRE = 2;
	static const int POISON = 3;
	static const int PARALYSIS = 4;
	static const int HUNGER = 5;
	static const int STARVATION = 6;
	static const int SLOW = 7;
	static const int OOZE = 8;
	static const int AMOK = 9;
	static const int TERROR = 10;
	static const int ROOTS = 11;
	static const int INVISIBLE = 12;
	static const int SHADOWS = 13;
	static const int WEAKNESS = 14;
	static const int FROST = 15;
	static const int BLINDNESS = 16;
	static const int COMBO = 17;
	static const int FURY = 18;
	static const int HEALING = 19;
	static const int ARMOR = 20;
	static const int HEART = 21;
	static const int LIGHT = 22;
	static const int CRIPPLE = 23;
	static const int BARKSKIN = 24;
	static const int IMMUNITY = 25;
	static const int BLEEDING = 26;
	static const int MARK = 27;
	static const int DEFERRED = 28;
	static const int VERTIGO = 29;
	static const int RAGE = 30;
	static const int SACRIFICE = 31;

	static const int SIZE = 7;

private:
	static BuffIndicator* heroInstance;

	SmartTexture* texture;
	TextureFilm* film;

	std::map<int,Image*> icons;

	Char* ch;

public:
	BuffIndicator(Char* ch);

	virtual void destroy();

	static void refreshHero();
protected:
	virtual void createChildren();
	virtual void layout();
};

