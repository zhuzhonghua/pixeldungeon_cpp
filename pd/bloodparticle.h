#pragma once

#include "pixelparticle.h"
#include "emitter.h"

class BloodParticle :public Shrinking{
public:
	static Emitter::Factory* FACTORY;

	BloodParticle();

	void reset(float x, float y);
	
	virtual void update();
};