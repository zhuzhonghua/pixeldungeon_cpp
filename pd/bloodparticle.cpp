#include "bloodparticle.h"
#include "util.h"

namespace{
	class FactoryNew :public Emitter::Factory{
	public:
		virtual void emit(Emitter* emitter, int index, float x, float y)
		{
			BloodParticle* bp = (BloodParticle*)emitter->recycle("BloodParticle");
			if (bp == NULL)
			{
				bp = new BloodParticle();
				emitter->add(bp);
			}
			bp->reset(x, y);
		}
	};
}
Emitter::Factory* BloodParticle::FACTORY = new FactoryNew();

BloodParticle::BloodParticle()
{
	tag = "BloodParticle";

	color(0xCC0000);
	_lifespan = 0.8f;

	GameMath::PointFSet(&acc, 0, +40);
	//acc.set(0, +40);
}

void BloodParticle::reset(float x, float y)
{
	revive();

	this->x = x;
	this->y = y;

	_left = _lifespan;

	_sizef = 4;

	GameMath::PointFSet(&speed, 0);
	//speed.set(0);
}

void BloodParticle::update()
{
	Shrinking::update();
	float p = _left / _lifespan;
	am = p > 0.6f ? (1 - p) * 2.5f : 1;
}
