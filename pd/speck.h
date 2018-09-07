#pragma once

#include <map>

#include "image.h"
#include "emitter.h"
#include "texturefilm.h"

class Speck :public Image{
public:
	static const int HEALING = 0;
	static const int STAR = 1;
	static const int LIGHT = 2;
	static const int QUESTION = 3;
	static const int UP = 4;
	static const int SCREAM = 5;
	static const int BONE = 6;
	static const int WOOL = 7;
	static const int ROCK = 8;
	static const int NOTE = 9;
	static const int CHANGE = 10;
	static const int HEART = 11;
	static const int BUBBLE = 12;
	static const int STEAM = 13;
	static const int COIN = 14;

	static const int DISCOVER = 101;
	static const int EVOKE = 102;
	static const int MASTERY = 103;
	static const int KIT = 104;
	static const int RATTLE = 105;
	static const int JET = 106;
	static const int TOXIC = 107;
	static const int PARALYSIS = 108;
	static const int DUST = 109;
	static const int FORGE = 110;
	static const int CONFUSION = 111;

private:
	static const int SIZE = 7;
	
	float lifespan;
	float left;

	static TextureFilm* film;

	static std::map<int, Emitter::Factory*> factories;

	static Emitter::Factory* get(int type);
public:
	int type;

	Speck();
	void reset(int index, float x, float y, int type);
	virtual void update();

	static Emitter::Factory* factory(int type);
	static Emitter::Factory* factory(int type, bool lightMode);
};