#include "pixelparticle.h"
#include "util.h"
#include "game.h"
#include "typedefine.h"
#include "dungeon.h"
#include "level.h"

PixelParticle::PixelParticle()
{
	_sizef = 0;
	_lifespan = 0;
	_left = 0;
	GameMath::PointFSet(&origin, +0.5f);
	//origin.set(+0.5f);
}

void PixelParticle::reset(float x, float y, int c, float s, float lifespan)
{
	revive();

	this->x = x;
	this->y = y;

	color(c);
	size(this->_sizef = s);

	this->_left = this->_lifespan = lifespan;
}

void PixelParticle::update()
{
	PseudoPixel::update();

	if ((_left -= Game::elapsed) <= 0) {
		kill();
	}
}

Shrinking::Shrinking()
{
	tag = "Shrinking";
}

void Shrinking::update()
{
	PixelParticle::update();
	size(_sizef * _left / _lifespan);
}

namespace{
	class SparkParticleFactory :public Emitter::Factory{
	public:
		virtual void emit(Emitter* emitter, int index, float x, float y)
		{
			SparkParticle* sp = (SparkParticle*)emitter->recycle<SparkParticle>();
			sp->reset(x, y);
		}

		virtual bool lightMode() { return true; };
	};
}

Emitter::Factory* SparkParticle::factory()
{
	if (FACTORY == NULL)
	{
		FACTORY = new SparkParticleFactory();
	}
	return FACTORY;
}

Emitter::Factory* SparkParticle::FACTORY;

SparkParticle::SparkParticle()
{
	size(2);

	GameMath::PointFSet(&acc, 0, +50);
	//acc.set(0, +50);
}

void SparkParticle::reset(float x, float y)
{
	revive();

	this->x = x;
	this->y = y;

	_left = _lifespan = Random::Float(0.5f, 1.0f);

	GameMath::PointFPolar(&speed, -Random::Float(3.1415926f), Random::Float(20, 40));
	//speed.polar(-Random.Float(3.1415926f), Random.Float(20, 40));
}

void SparkParticle::update()
{
	PixelParticle::update();
	size(Random::Float(5 * _left / _lifespan));
}

namespace{
	class ElmoParticleFactory :public Emitter::Factory{
	public:
		virtual void emit(Emitter* emitter, int index, float x, float y)
		{
			ElmoParticle* ep = (ElmoParticle*)emitter->recycle<ElmoParticle>();
			ep->reset(x, y);
		}
		virtual bool lightMode() {
			return true;
		}
	};
}

Emitter::Factory* ElmoParticle::factory()
{
	if (FACTORY == NULL)
	{
		FACTORY = new ElmoParticleFactory();
	}
	return FACTORY;
}

Emitter::Factory* ElmoParticle::FACTORY;

ElmoParticle::ElmoParticle()
{
}

void ElmoParticle::reset(float x, float y)
{
	revive();

	this->x = x;
	this->y = y;

	_left = _lifespan;

	_sizef = 4;
	GameMath::PointFSet(&speed, 0);
	//speed.set(0);
}

void ElmoParticle::update()
{
	Shrinking::update();
	float p = _left / _lifespan;
	am = p > 0.8f ? (1 - p) * 5 : 1;
}

namespace{
	class ShadowParticleMissileFactory :public Emitter::Factory{
	public:
		virtual void emit(Emitter* emitter, int index, float x, float y)
		{
			ShadowParticle* ep = (ShadowParticle*)emitter->recycle<ShadowParticle>();
			ep->reset(x, y);
		}
	};
	class ShadowParticleCurseFactory :public Emitter::Factory{
	public:
		virtual void emit(Emitter* emitter, int index, float x, float y)
		{
			ShadowParticle* ep = (ShadowParticle*)emitter->recycle<ShadowParticle>();
			ep->resetCurse(x, y);
		}
	};
	class ShadowParticleUpFactory :public Emitter::Factory{
	public:
		virtual void emit(Emitter* emitter, int index, float x, float y)
		{
			ShadowParticle* ep = (ShadowParticle*)emitter->recycle<ShadowParticle>();
			ep->resetUp(x, y);
		}
	};
}
Emitter::Factory* ShadowParticle::missile()
{
	if (MISSILE == NULL) MISSILE = new ShadowParticleMissileFactory();
	return MISSILE;
}
Emitter::Factory* ShadowParticle::curse()
{
	if (CURSE == NULL) CURSE = new ShadowParticleCurseFactory();
	return CURSE;
}
Emitter::Factory* ShadowParticle::up()
{
	if (UP == NULL) UP = new ShadowParticleUpFactory();
	return UP;
}

Emitter::Factory* ShadowParticle::MISSILE;
Emitter::Factory* ShadowParticle::CURSE;
Emitter::Factory* ShadowParticle::UP;

ShadowParticle::ShadowParticle()
{
}

void ShadowParticle::reset(float x, float y)
{
	revive();

	this->x = x;
	this->y = y;

	GameMath::PointFSet(&speed, Random::Float(-5, +5), Random::Float(-5, +5));
	//speed.set(Random.Float(-5, +5), Random.Float(-5, +5));

	_sizef = 6;
	_left = _lifespan = 0.5f;
}

void ShadowParticle::resetCurse(float x, float y)
{
	revive();

	_sizef = 8;
	_left = _lifespan = 0.5f;

	GameMath::PointFPolar(&speed, Random::Float(GameMath::POINTF_PI2), Random::Float(16, 32));
	//speed.polar(Random.Float(PointF.PI2), Random.Float(16, 32));
	this->x = x - speed.x * _lifespan;
	this->y = y - speed.y * _lifespan;
}

void ShadowParticle::resetUp(float x, float y)
{
	revive();

	GameMath::PointFSet(&speed, Random::Float(-8, +8), Random::Float(-32, -48));
	//speed.set(Random::Float(-8, +8), Random::Float(-32, -48));
	this->x = x;
	this->y = y;

	_sizef = 6;
	_left = _lifespan = 1.0f;
}

void ShadowParticle::update()
{
	Shrinking::update();

	float p = _left / _lifespan;
	// alpha: 0 -> 1 -> 0; size: 6 -> 0; color: 0x660044 -> 0x000000
	color(ColorMath::interpolate(0x000000, 0x440044, p));
	am = p < 0.5f ? p * p * 4 : (1 - p) * 2;
}

PoisonParticle::PoisonParticle()
{
	_lifespan = 0.6f;

	GameMath::PointFSet(&acc, 0, 30);
	//acc.set(0, +30);
}

void PoisonParticle::resetMissile(float x, float y)
{
	revive();

	this->x = x;
	this->y = y;

	_left = _lifespan;

	GameMath::PointFPolar(&speed, -Random::Float(3.1415926f), Random::Float(6));
	//speed.polar(-Random.Float(3.1415926f), Random.Float(6));
}

void PoisonParticle::resetSplash(float x, float y)
{
	revive();

	this->x = x;
	this->y = y;

	_left = _lifespan;

	GameMath::PointFPolar(&speed, Random::Float(3.1415926f), Random::Float(10, 20));
	//speed.polar(Random.Float(3.1415926f), Random.Float(10, 20));
}

void PoisonParticle::update()
{
	PixelParticle::update();
	// alpha: 1 -> 0; size: 1 -> 4
	size(4 - (am = _left / _lifespan) * 3);
	// color: 0x8844FF -> 0x00FF00
	color(ColorMath::interpolate(0x00FF00, 0x8844FF, am));
}


namespace{
	class PoisonParticleMissileFactory :public Emitter::Factory{
	public:
		virtual void emit(Emitter* emitter, int index, float x, float y)
		{
			PoisonParticle* pp = (PoisonParticle*)emitter->recycle<PoisonParticle>();
			//RECYCLE2(pp, emitter, PoisonParticle);
			pp->resetMissile(x, y);
		}
		virtual boolean lightMode() {
			return true;
		};
	};
	class PoisonParticleSplashFactory :public Emitter::Factory{
	public:
		virtual void emit(Emitter* emitter, int index, float x, float y)
		{
			PoisonParticle* pp = (PoisonParticle*)emitter->recycle<PoisonParticle>();
			//RECYCLE2(pp, emitter, PoisonParticle);
			pp->resetSplash(x, y);
		}
		virtual boolean lightMode() {
			return true;
		};
	};
}

Emitter::Factory* PoisonParticle::missile()
{
	if (MISSILE == NULL) MISSILE = new PoisonParticleMissileFactory();
	return MISSILE;
}
Emitter::Factory* PoisonParticle::splash()
{
	if (SPLASH == NULL) SPLASH = new PoisonParticleSplashFactory();
	return SPLASH;
}

Emitter::Factory* PoisonParticle::MISSILE;
Emitter::Factory* PoisonParticle::SPLASH;

int LeafParticle::color1;
int LeafParticle::color2;

namespace{
	class LeafParticleGeneralFactory :public Emitter::Factory{
	public:
		virtual void emit(Emitter* emitter, int index, float x, float y)
		{
			LeafParticle* p = (LeafParticle*)emitter->recycle<LeafParticle>();
			//RECYCLE2(p, emitter, LeafParticle);
			p->color(ColorMath::random(0x004400, 0x88CC44));
			p->reset(x, y);
		}
	};
	class LeafParticleLevelSpecificFactory :public Emitter::Factory{
	public:
		virtual void emit(Emitter* emitter, int index, float x, float y)
		{
			LeafParticle* p = (LeafParticle*)emitter->recycle<LeafParticle>();
			//RECYCLE2(p, emitter, LeafParticle);
			p->color(ColorMath::random(Dungeon::level->color1, Dungeon::level->color2));
			p->reset(x, y);
		}
	};
}

Emitter::Factory* LeafParticle::general()
{
	if(GENERAL == NULL) GENERAL = new LeafParticleGeneralFactory;
	return GENERAL;
}
Emitter::Factory* LeafParticle::levelSpecific()
{
	if (LEVEL_SPECIFIC == NULL) LEVEL_SPECIFIC = new LeafParticleLevelSpecificFactory();
	return LEVEL_SPECIFIC;
}

Emitter::Factory* LeafParticle::GENERAL;
Emitter::Factory* LeafParticle::LEVEL_SPECIFIC;

LeafParticle::LeafParticle()
{
	_lifespan = 1.2f;
	GameMath::PointFSet(&acc, 0, 25);
	//acc.set(0, 25);
}

void LeafParticle::reset(float x, float y)
{
	revive();

	this->x = x;
	this->y = y;

	GameMath::PointFSet(&speed, Random::Float(-8, +8), -20);
	//speed.set(Random.Float(-8, +8), -20);

	_left = _lifespan;
	_sizef = Random::Float(2, 3);
}

namespace{
	class SnowParticleFactory :public Emitter::Factory{
	public:
		virtual void emit(Emitter* emitter, int index, float x, float y)
		{
			SnowParticle* p = (SnowParticle*)emitter->recycle<SnowParticle>();
			//RECYCLE2(p, emitter, SnowParticle);
			p->reset(x, y);
		}
	};
}

Emitter::Factory* SnowParticle::factory()
{
	if (FACTORY == NULL) FACTORY = new SnowParticleFactory();
	return FACTORY;
}

Emitter::Factory* SnowParticle::FACTORY;

SnowParticle::SnowParticle()
{
	GameMath::PointFSet(&speed, 0, Random::Float(5, 8));
	//speed.set(0, Random.Float(5, 8));
	_lifespan = 1.2f;
}

void SnowParticle::reset(float x, float y)
{
	revive();

	this->x = x;
	this->y = y - speed.y * _lifespan;

	_left = _lifespan;
}

void SnowParticle::update()
{
	PixelParticle::update();
	float p = _left / _lifespan;
	am = (p < 0.5f ? p : 1 - p) * 1.5f;
}

namespace{
	class ShaftParticleFactory :public Emitter::Factory{
	public:
		virtual void emit(Emitter* emitter, int index, float x, float y)
		{
			ShaftParticle* p = (ShaftParticle*)emitter->recycle<ShaftParticle>();
			//RECYCLE2(p, emitter, ShaftParticle);
			p->reset(x, y);
		}
		virtual boolean lightMode() {
			return true;
		}
	};
}


Emitter::Factory* ShaftParticle::factory()
{
	if (FACTORY == NULL) FACTORY = new ShaftParticleFactory();
	return FACTORY;
}
Emitter::Factory* ShaftParticle::FACTORY;

ShaftParticle::ShaftParticle()
{
	_lifespan = 1.2f;
	GameMath::PointFSet(&speed, 0, -6);
	//speed.set(0, -6);
}

void ShaftParticle::reset(float x, float y)
{
	revive();

	this->x = x;
	this->y = y;

	offs = -Random::Float(_lifespan);
	_left = _lifespan - offs;
}

void ShaftParticle::update()
{
	PixelParticle::update();

	float p = _left / _lifespan;
	am = p < 0.5f ? p : 1 - p;
	scale.x = (1 - p) * 4;
	scale.y = 16 + (1 - p) * 16;
}

namespace{
	class EarthParticleFactory :public Emitter::Factory{
	public:
		virtual void emit(Emitter* emitter, int index, float x, float y)
		{
			EarthParticle* p = (EarthParticle*)emitter->recycle<EarthParticle>();
			//RECYCLE2(p, emitter, EarthParticle);
			p->reset(x, y);
		}
	};
}


Emitter::Factory* EarthParticle::factory()
{
	if (FACTORY == NULL) FACTORY = new EarthParticleFactory();
	return FACTORY;
}
Emitter::Factory* EarthParticle::FACTORY;
EarthParticle::EarthParticle()
{
	color(ColorMath::random(0x444444, 0x777766));
	angle = Random::Float(-30, 30);

	_lifespan = 0.5f;
}

void EarthParticle::reset(float x, float y)
{
	revive();

	this->x = x;
	this->y = y;

	_left = _lifespan;
}

void EarthParticle::update()
{
	PixelParticle::update();

	float p = _left / _lifespan;
	size((p < 0.5f ? p : 1 - p) * 16);
}

BlastParticle::BlastParticle()
{
	color(0xEE7722);
	//acc.set(0, +50);
	GameMath::PointFSet(&acc, 0, +50);
}

void BlastParticle::reset(float x, float y)
{
	revive();

	this->x = x;
	this->y = y;

	_left = _lifespan = Random::Float();

	_sizef = 8;
	//speed.polar(-Random::Float(3.1415926f), Random::Float(32, 64));
	GameMath::PointFPolar(&speed, -Random::Float(3.1415926f), Random::Float(32, 64));
}

namespace{
	class BlastParticleFactory:public Emitter::Factory {
	public:
		virtual void emit(Emitter* emitter, int index, float x, float y)
		{
			BlastParticle* bp = (BlastParticle*)emitter->recycle<BlastParticle>();
			//RECYCLE2(bp, emitter, BlastParticle);
			bp->reset(x, y);
		}
		virtual boolean lightMode() 
		{
			return true;
		};
	};
}

Emitter::Factory* BlastParticle::factory()
{
	if (FACTORY == NULL) FACTORY = new BlastParticleFactory();
	return FACTORY;
}
Emitter::Factory* BlastParticle::FACTORY;
