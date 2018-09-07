#include "splash.h"
#include "gamescene.h"
#include "dungeontilemap.h"
#include "pixelparticle.h"

Splash::SplashFactory* Splash::FACTORY = new Splash::SplashFactory();

void Splash::at(int cell, int color, int n)
{
	at(DungeonTilemap::tileCenterToWorld(cell), color, n);
}

void Splash::at(PointF p, int color, int n)
{
	if (n <= 0) 
	{
		return;
	}

	Emitter* emitter = GameScene::emitter();
	emitter->pos(p);

	FACTORY->color = color;
	FACTORY->dir = -3.1415926f / 2;
	FACTORY->cone = 3.1415926f;
	emitter->burst(FACTORY, n);
}

void Splash::at(PointF p, float dir, float cone, int color, int n)
{
	if (n <= 0) 
	{
		return;
	}

	Emitter* emitter = GameScene::emitter();
	emitter->pos(p);

	FACTORY->color = color;
	FACTORY->dir = dir;
	FACTORY->cone = cone;
	emitter->burst(FACTORY, n);
}

void Splash::SplashFactory::emit(Emitter* emitter, int index, float x, float y)
{
	PixelParticle* p = (PixelParticle*)emitter->recycle("Shrinking");
	if (p == NULL)
	{
		p = new Shrinking();
		emitter->add(p);
	}
	p->reset(x, y, color, 4, Random::Float(0.5f, 1.0f));

	GameMath::PointFPolar(&p->speed, Random::Float(dir - cone / 2, dir + cone / 2), Random::Float(40, 80));
	//p->speed.polar();
	GameMath::PointFSet(&p->acc, 0, +100);
	//p->acc.set(0, +100);
}