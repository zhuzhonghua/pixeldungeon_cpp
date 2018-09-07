#include "flameparticle.h"
#include "util.h"

namespace{
	class FlameParticleFactoryNew :public Emitter::Factory{
	public:
		virtual void emit(Emitter* emitter, int index, float x, float y)
		{
			FlameParticle* fp = (FlameParticle*)emitter->recycle("FlameParticle");
			if (fp == NULL)
			{
				fp = new FlameParticle();
				emitter->add(fp);
			}
			fp->reset(x, y);
		}

		virtual bool lightMode(){ return true; }
	};
}
Emitter::Factory* FlameParticle::FACTORY = new FlameParticleFactoryNew();

FlameParticle::FlameParticle()
{
	tag = "FlameParticle";

	color(0xEE7722);
	_lifespan = 0.6f;

	GameMath::PointFSet(&acc, 0, -80);
	//acc.set(0, -80);
}

void FlameParticle::reset(float x, float y)
{
	revive();

	this->x = x;
	this->y = y;

	_left = _lifespan;

	_sizef = 4;

	GameMath::PointFSet(&speed, 0);
	//speed.set(0);
}

void FlameParticle::update()
{
	Shrinking::update();

	float p = _left / _lifespan;
	am = p > 0.8f ? (1 - p) * 5 : 1;
}
