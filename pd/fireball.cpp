#include "stdafx.h"
#include "fireball.h"
#include "define.h"
#include "util.h"
#include "game.h"
#include "pixelparticle.h"

float Flame::LIFESPAN = 1.0f;
float Flame::SPEED = -40.0f;
float Flame::ACC = -20.0f;

Flame::Flame()
{
	init();
	tag = "Flame";
}

void Flame::init()
{
	texture(Assets::FIREBALL);

	frame(Random::Int(0, 2) == 0 ? Fireball_FLAME1 : Fireball_FLAME2);
	GameMath::PointFSet(&origin, width / 2, height / 2);
	//origin.set(width / 2, height / 2);
	GameMath::PointFSet(&acc, 0, ACC);
	//acc.set(0, ACC);
}

void Flame::reset()
{
	revive();
	timeLeft = LIFESPAN;
	GameMath::PointFSet(&speed, 0, SPEED);
	//speed.set(0, SPEED);
}

void Flame::update()
{
	Image::update();

	if ((timeLeft -= Game::elapsed) <= 0) 
	{
		kill();
	}
	else 
	{
		float p = timeLeft / LIFESPAN;
		GameMath::PointFSet(&scale, p);
		//scale.set(p);
		alpha(p > 0.8f ? (1 - p) * 5.0f : p * 1.25f);
	}
}

void EmitterFactory1::emit(Emitter* emitter, int index, float x, float y)
{
	Flame* p = (Flame*)emitter->recycle("Flame");
	if (p == NULL)
	{
		p = (Flame*)emitter->add(new Flame());
	}
	p->reset();
	p->x = x - p->width / 2;
	p->y = y - p->height / 2;
}

Fireball::Fireball()
{
	init();
}

void Fireball::createChildren()
{
	_sparks = new Group();
	add(_sparks);

	_bLight = new Image(Assets::FIREBALL);
	_bLight->frame(Fireball_BLIGHT);	
	GameMath::PointFSet(&_bLight->origin, _bLight->width / 2);
	//bLight.origin.set( bLight.width / 2 );
	_bLight->angularSpeed = -90;
	add(_bLight);
	
	_emitter = new Emitter();
	_emitter->pour(new EmitterFactory1(), 0.1f);
	add(_emitter);
	
	_fLight = new Image(Assets::FIREBALL);
	_fLight->frame(Fireball_FLIGHT);
	GameMath::PointFSet(&_fLight->origin, _fLight->width / 2);
	//_fLight->origin.set(_fLight.width / 2);
	_fLight->angularSpeed = 360;
	add(_fLight);
	//
	_bLight->tex->filter(Texture::LINEAR, Texture::LINEAR);	
}

void Fireball::layout()
{
	_bLight->x = _x - _bLight->width / 2;
	_bLight->y = _y - _bLight->height / 2;
	
	_emitter->pos(
		_x - _bLight->width / 4,
		_y - _bLight->height / 4,
		_bLight->width / 2,
		_bLight->height / 2);
	
	_fLight->x = _x - _fLight->width / 2;
	_fLight->y = _y - _fLight->height / 2;
}

void Fireball::update()
{
	Component::update();

	if (Random::Float(0, 1) < Game::elapsed) 
	{
		PixelParticle* spark = (PixelParticle*)_sparks->recycle("Shrinking");
		if (spark == NULL)
		{
			//spark = (PixelParticle*)_sparks->add(new Shrinking());
			spark = new Shrinking();
		}
		spark->reset(_x, _y, ColorMath::random(COLOR, 0x66FF66), 2, Random::Float(0.5f, 1.0f));
		GameMath::PointFSet(&spark->speed, Random::Float(-40, +40), Random::Float(-60, +20));
		//spark->speed.set(
		//	Random::Float(-40, +40),
		//	Random::Float(-60, +20));
		GameMath::PointFSet(&spark->acc, 0, 80);
		//spark->acc.set(0, +80);
		_sparks->add(spark);
	}
}