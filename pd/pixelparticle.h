#pragma once

#include "pseudopixel.h"
#include "util.h"

#include "emitter.h"

class PixelParticle :public PseudoPixel{
protected:
	float _sizef;

	float _lifespan;
	float _left;
public:
	PixelParticle();

	void reset(float x, float y, int c, float s, float lifespan);
	virtual void update();
};

class Shrinking :public PixelParticle {
public:
	Shrinking();
	virtual void update();
};

class SparkParticle :public PixelParticle{
public:
	static Emitter::Factory* FACTORY;

	SparkParticle();
	void reset(float x, float y);
	virtual void update();
};

class ElmoParticle :public Shrinking{
public:
	static Emitter::Factory* FACTORY;

	ElmoParticle();
	void reset(float x, float y);

	virtual void update();
};

class ShadowParticle :public Shrinking{
public:
	static Emitter::Factory* MISSILE;
	static Emitter::Factory* CURSE;
	static Emitter::Factory* UP;

	ShadowParticle();

	void reset(float x, float y);
	void resetCurse(float x, float y);
	void resetUp(float x, float y);
	virtual void update();
};

class PoisonParticle :public PixelParticle{
public:
	static Emitter::Factory* MISSILE;
	static Emitter::Factory* SPLASH;

	PoisonParticle();
	void resetMissile(float x, float y);
	void resetSplash(float x, float y);

	virtual void update();
};

class LeafParticle :public Shrinking{
public:
	static int color1;
	static int color2;
	static Emitter::Factory* GENERAL;
	static Emitter::Factory* LEVEL_SPECIFIC;

	LeafParticle();

	void reset(float x, float y);
};

class SnowParticle :public PixelParticle{
public:
	static Emitter::Factory* FACTORY;

	SnowParticle();

	void reset(float x, float y);
	virtual void update();
};

class ShaftParticle :public PixelParticle{
private:
	float offs;
public:
	static Emitter::Factory* FACTORY;
	ShaftParticle();
	void reset(float x, float y);
	virtual void update();
};

class EarthParticle :public PixelParticle{
public:
	static Emitter::Factory* FACTORY;
	EarthParticle();
	void reset(float x, float y);
	virtual void update();
};

class BlastParticle :public Shrinking{
public:
	BlastParticle();
	void reset(float x, float y);
	virtual void update()
	{
		Shrinking::update();
		am = _left > 0.8f ? (1 - _left) * 5 : 1;
	}

	static Emitter::Factory* FACTORY;
};