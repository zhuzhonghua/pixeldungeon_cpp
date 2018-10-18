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
	static Emitter::Factory* FACTORY;
public:	
	static Emitter::Factory* factory();

	SparkParticle();
	void reset(float x, float y);
	virtual void update();
};

class ElmoParticle :public Shrinking{
	static Emitter::Factory* FACTORY;
public:
	static Emitter::Factory* factory();

	ElmoParticle();
	void reset(float x, float y);

	virtual void update();
};

class ShadowParticle :public Shrinking{
	static Emitter::Factory* MISSILE;
	static Emitter::Factory* CURSE;
	static Emitter::Factory* UP;
public:
	static Emitter::Factory* missile();
	static Emitter::Factory* curse();
	static Emitter::Factory* up();

	ShadowParticle();

	void reset(float x, float y);
	void resetCurse(float x, float y);
	void resetUp(float x, float y);
	virtual void update();
};

class PoisonParticle :public PixelParticle{

	static Emitter::Factory* MISSILE;
	static Emitter::Factory* SPLASH;
public:

	static Emitter::Factory* missile();
	static Emitter::Factory* splash();

	PoisonParticle();
	void resetMissile(float x, float y);
	void resetSplash(float x, float y);

	virtual void update();
};

class LeafParticle :public Shrinking{
	static Emitter::Factory* GENERAL;
	static Emitter::Factory* LEVEL_SPECIFIC;
public:
	static int color1;
	static int color2;

	static Emitter::Factory* general();
	static Emitter::Factory* levelSpecific();

	LeafParticle();

	void reset(float x, float y);
};

class SnowParticle :public PixelParticle{
	static Emitter::Factory* FACTORY;
public:
	static Emitter::Factory* factory();

	SnowParticle();

	void reset(float x, float y);
	virtual void update();
};

class ShaftParticle :public PixelParticle{
	static Emitter::Factory* FACTORY;
private:
	float offs;
public:
	static Emitter::Factory* factory();
	ShaftParticle();
	void reset(float x, float y);
	virtual void update();
};

class EarthParticle :public PixelParticle{
	static Emitter::Factory* FACTORY;
public:
	static Emitter::Factory* factory();
	EarthParticle();
	void reset(float x, float y);
	virtual void update();
};

class BlastParticle :public Shrinking{
	static Emitter::Factory* FACTORY;
public:
	BlastParticle();
	void reset(float x, float y);
	virtual void update()
	{
		Shrinking::update();
		am = _left > 0.8f ? (1 - _left) * 5 : 1;
	}

	static Emitter::Factory* factory();
};