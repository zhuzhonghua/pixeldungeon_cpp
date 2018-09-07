#include "speck.h"
#include "define.h"
#include "util.h"
#include "game.h"

TextureFilm* Speck::film;
std::map<int, Emitter::Factory*> Speck::factories;

Speck::Speck()
{
	tag = "Speck";

	texture(Assets::SPECKS);
	if (film == NULL) 
	{
		film = new TextureFilm(tex, SIZE, SIZE);
	}

	GameMath::PointFSet(&origin, SIZE / 2.0f);
	//origin.set(SIZE / 2f);
}

void Speck::reset(int index, float x, float y, int type)
{
	revive();

	this->type = type;
	switch (type) 
	{
	case DISCOVER:
		frame(film->get(LIGHT));
		break;
	case EVOKE:
	case MASTERY:
	case KIT:
	case FORGE:
		frame(film->get(STAR));
		break;
	case RATTLE:
		frame(film->get(BONE));
		break;
	case JET:
	case TOXIC:
	case PARALYSIS:
	case CONFUSION:
	case DUST:
		frame(film->get(STEAM));
		break;
	default:
		frame(film->get(type));
	}

	this->x = x - origin.x;
	this->y = y - origin.y;

	resetColor();
	GameMath::PointFSet(&scale, 1);
	//scale.set(1);
	GameMath::PointFSet(&speed, 1);
	//speed.set(0);
	GameMath::PointFSet(&acc, 1);
	//acc.set(0);
	angle = 0;
	angularSpeed = 0;

	switch (type) 
	{

	case HEALING:
		GameMath::PointFSet(&speed, 1);
		//speed.set(0, -20);
		lifespan = 1.0f;
		break;

	case STAR:
		GameMath::PointFPolar(&speed, Random::Float(2 * 3.1415926f), Random::Float(128));
		//speed.polar(Random::Float(2 * 3.1415926f), Random::Float(128));
		GameMath::PointFSet(&acc, 0, 128);
		//acc.set(0, 128);
		angle = Random::Float(360);
		angularSpeed = Random::Float(-360, +360);
		lifespan = 1.0f;
		break;

	case FORGE:
		GameMath::PointFPolar(&speed, -Random::Float(3.1415926f), Random::Float(64));
		//speed.polar(-Random::Float(3.1415926f), Random::Float(64));
		GameMath::PointFSet(&acc, 0, 128);
		//acc.set(0, 128);
		angle = Random::Float(360);
		angularSpeed = Random::Float(-360, +360);
		lifespan = 0.51f;
		break;

	case EVOKE:
		GameMath::PointFPolar(&speed, -Random::Float(3.1415926f), 50);
		//speed.polar(-Random::Float(3.1415926f), 50);
		GameMath::PointFSet(&acc, 0, 50);
		//acc.set(0, 50);
		angle = Random::Float(360);
		angularSpeed = Random::Float(-180, +180);
		lifespan = 1.0f;
		break;

	case KIT:
		GameMath::PointFPolar(&speed, index * 3.1415926f / 5, 50);
		//speed.polar(index * 3.1415926f / 5, 50);
		GameMath::PointFSet(&acc, -speed.x, -speed.y);
		//acc.set(-speed.x, -speed.y);
		angle = index * 36;
		angularSpeed = 360;
		lifespan = 1.0f;
		break;

	case MASTERY:
		GameMath::PointFSet(&speed, Random::Int(2) == 0 ? Random::Float(-128, -64) : Random::Float(+64, +128), 0);
		//speed.set(Random::Int(2) == 0 ? Random::Float(-128, -64) : Random::Float(+64, +128), 0);
		angularSpeed = speed.x < 0 ? -180 : +180;

		GameMath::PointFSet(&acc, -speed.x, 0);
		//acc.set(-speed.x, 0);
		lifespan = 0.5f;
		break;

	case LIGHT:
		angle = Random::Float(360);
		angularSpeed = 90;
		lifespan = 1.0f;
		break;

	case DISCOVER:
		angle = Random::Float(360);
		angularSpeed = 90;
		lifespan = 0.5f;
		am = 0;
		break;

	case QUESTION:
		lifespan = 0.8f;
		break;

	case UP:
		GameMath::PointFSet(&speed, 0, -20);
		//speed.set(0, -20);
		lifespan = 1.0f;
		break;

	case SCREAM:
		lifespan = 0.9f;
		break;

	case BONE:
		lifespan = 0.2f;
		GameMath::PointFPolar(&speed, Random::Float(2 * 3.1415926f), 24 / lifespan);
		//speed.polar(Random::Float(2 * 3.1415926f), 24 / lifespan);
		GameMath::PointFSet(&acc, 0, 128);
		//acc.set(0, 128);
		angle = Random::Float(360);
		angularSpeed = 360;
		break;

	case RATTLE:
		lifespan = 0.5f;
		GameMath::PointFSet(&speed, 0, -200);
		//speed.set(0, -200);
		GameMath::PointFSet(&acc, 0, -200);
		//acc.set(0, -2 * speed.y / lifespan);
		angle = Random::Float(360);
		angularSpeed = 360;
		break;

	case WOOL:
		lifespan = 0.5f;
		GameMath::PointFSet(&speed, 0, -50);
		//speed.set(0, -50);
		angle = Random::Float(360);
		angularSpeed = Random::Float(-360, +360);
		break;

	case ROCK:
		angle = Random::Float(360);
		angularSpeed = Random::Float(-360, +360);
		GameMath::PointFSet(&scale, Random::Float(1, 2));
		//scale.set(Random::Float(1, 2));
		GameMath::PointFSet(&speed, 0, 64);
		//speed.set(0, 64);
		lifespan = 0.2f;
		y -= speed.y * lifespan;
		break;

	case NOTE:
		angularSpeed = Random::Float(-30, +30);
		GameMath::PointFPolar(&speed, (angularSpeed - 90) * GameMath::POINTF_G2R, 30);
		//speed.polar((angularSpeed - 90) * PointF.G2R, 30);
		lifespan = 1.0f;
		break;

	case CHANGE:
		angle = Random::Float(360);
		GameMath::PointFPolar(&speed, (angle - 90) * GameMath::POINTF_G2R, Random::Float(4, 12));
		//speed.polar((angle - 90) * PointF.G2R, Random::Float(4, 12));
		lifespan = 1.5f;
		break;

	case HEART:
		GameMath::PointFSet(&speed, Random::Int(-10, +10), -40);
		//speed.set(Random::Int(-10, +10), -40);
		angularSpeed = Random::Float(-45, +45);
		lifespan = 1.0f;
		break;

	case BUBBLE:
		GameMath::PointFSet(&speed, 0, -15);
		//speed.set(0, -15);
		GameMath::PointFSet(&scale, Random::Float(0.8f, 1));
		//scale.set(Random::Float(0.8f, 1));
		lifespan = Random::Float(0.8f, 1.5f);
		break;

	case STEAM:
		speed.y = -Random::Float(20, 30);
		angularSpeed = Random::Float(+180);
		angle = Random::Float(360);
		lifespan = 1.0f;
		break;

	case JET:
		speed.y = +32;
		acc.y = -64;
		angularSpeed = Random::Float(180, 360);
		angle = Random::Float(360);
		lifespan = 0.5f;
		break;

	case TOXIC:
		hardlight(0x50FF60);
		angularSpeed = 30;
		angle = Random::Float(360);
		lifespan = Random::Float(1.0f, 3.0f);
		break;

	case PARALYSIS:
		hardlight(0xFFFF66);
		angularSpeed = -30;
		angle = Random::Float(360);
		lifespan = Random::Float(1.0f, 3.0f);
		break;

	case CONFUSION:
		hardlight(Random::Int(0x1000000) | 0x000080);
		angularSpeed = Random::Float(-20, +20);
		angle = Random::Float(360);
		lifespan = Random::Float(1.0f, 3.0f);
		break;

	case DUST:
		hardlight(0xFFFF66);
		angle = Random::Float(360);
		GameMath::PointFPolar(&speed, Random::Float(2 * 3.1415926f), Random::Float(16, 48));
		//speed.polar(Random::Float(2 * 3.1415926f), Random::Float(16, 48));
		lifespan = 0.5f;
		break;

	case COIN:
		GameMath::PointFPolar(&speed, -GameMath::POINTF_PI * Random::Float(0.3f, 0.7f), Random::Float(48, 96));
		//speed.polar(-PointF.PI * Random::Float(0.3f, 0.7f), Random::Float(48, 96));
		acc.y = 256;
		lifespan = -speed.y / acc.y * 2;
		break;
	}

	left = lifespan;
}

void Speck::update()
{
	Image::update();

	left -= Game::elapsed;
	if (left <= 0) 
	{
		kill();
	}
	else 
	{
		float p = 1 - left / lifespan;	// 0 -> 1

		switch (type) 
		{
		case STAR:
		case FORGE:
			GameMath::PointFSet(&scale, 1 - p);
			//scale.set(1 - p);
			am = p < 0.2f ? p * 5.0f : (1 - p) * 1.25f;
			break;

		case KIT:
		case MASTERY:
			am = 1 - p * p;
			break;

		case EVOKE:

		case HEALING:
			am = p < 0.5f ? 1 : 2 - p * 2;
			break;

		case LIGHT:
			am = GameMath::PointFSet(&scale, p < 0.2f ? p * 5.0f : (1 - p) * 1.25f)->x;
			//am = scale.set(p < 0.2f ? p * 5f : (1 - p) * 1.25f).x;
			break;

		case DISCOVER:
			am = 1 - p;
			GameMath::PointFSet(&scale, (p < 0.5f ? p : 1 - p) * 2);
			//scale.set((p < 0.5f ? p : 1 - p) * 2);
			break;

		case QUESTION:
			GameMath::PointFSet(&scale, (float)(sqrt(p < 0.5f ? p : 1 - p) * 3));
			//scale.set((float)(Math.sqrt(p < 0.5f ? p : 1 - p) * 3));
			break;

		case UP:
			GameMath::PointFSet(&scale, (float)(sqrt(p < 0.5f ? p : 1 - p) * 2));
			//scale.set((float)(Math.sqrt(p < 0.5f ? p : 1 - p) * 2));
			break;

		case SCREAM:
			am = (float)sqrt((p < 0.5f ? p : 1 - p) * 2.0f);
			GameMath::PointFSet(&scale, p * 7);
			//scale.set(p * 7);
			break;

		case BONE:
		case RATTLE:
			am = p < 0.9f ? 1 : (1 - p) * 10;
			break;

		case ROCK:
			am = p < 0.2f ? p * 5 : 1;
			break;

		case NOTE:
			am = 1 - p * p;
			break;

		case WOOL:
			GameMath::PointFSet(&scale, 1 - p);
			//scale.set(1 - p);
			break;

		case CHANGE:
			am = (float)sqrt((p < 0.5f ? p : 1 - p) * 2);
			scale.y = (1 + p) * 0.5f;
			scale.x = scale.y * cos(left * 15);
			break;

		case HEART:
			GameMath::PointFSet(&scale, 1 - p);
			//scale.set(1 - p);
			am = 1 - p * p;
			break;

		case BUBBLE:
			am = p < 0.2f ? p * 5 : 1;
			break;

		case STEAM:
		case TOXIC:
		case PARALYSIS:
		case CONFUSION:
		case DUST:
			am = p < 0.5f ? p : 1 - p;
			GameMath::PointFSet(&scale, 1 + p * 2);
			//scale.set(1 + p * 2);
			break;

		case JET:
			am = (p < 0.5f ? p : 1 - p) * 2;
			GameMath::PointFSet(&scale, p * 1.5f);
			//scale.set(p * 1.5f);
			break;

		case COIN:
			scale.x = cos(left * 5);
			rm = gm = bm = (abs(scale.x) + 1) * 0.5f;
			am = p < 0.9f ? 1 : (1 - p) * 10;
			break;
		}
	}
}

Emitter::Factory* Speck::factory(int type)
{
	return factory(type, false);
}

Emitter::Factory* Speck::get(int type)
{
	std::map<int, Emitter::Factory*>::iterator itr = factories.find(type);
	if (itr != factories.end()) return itr->second;
	return NULL;
}

namespace{
	class TempEmitterFactory1 :public Emitter::Factory{
	public:
		int type;
		bool _lightMode;
		TempEmitterFactory1(int p, bool p2)
		{
			type = p;
			_lightMode = p2;
		}
		void emit(Emitter* emitter, int index, float x, float y)
		{
			Speck* p = (Speck*)emitter->recycle("Speck");
			if (p == NULL)
			{
				//spark = (PixelParticle*)_sparks->add(new Shrinking());
				p = new Speck();
				emitter->add(p);
			}
			p->reset(index, x, y, type);
		}
		bool lightMode() { return _lightMode; }
	};
}
Emitter::Factory* Speck::factory(int type, bool lightMode)
{
	Emitter::Factory* factory = get(type);

	if (factory == NULL) 
	{
		factory = new TempEmitterFactory1(type, lightMode);
		factories.insert(std::make_pair(type, factory));
	}

	return factory;
}