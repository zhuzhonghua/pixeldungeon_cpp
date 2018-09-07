#include "mobsprite.h"
#include "mob.h"
#include "texturefilm.h"
#include "dungeon.h"
#include "speck.h"
#include "alphatweener.h"
#include "gamescene.h"
#include "dungeontilemap.h"
#include "lightning.h"
#include "shaman.h"
#include "herosprite.h"
#include "hero.h"

const float MobSprite::FADE_TIME = 3.0f;
const float MobSprite::FALL_TIME = 1.0f;

void MobSprite::update()
{
	sleeping = ch != NULL && ((Mob*)ch)->state == ((Mob*)ch)->SLEEPEING;
	CharSprite::update();
}

namespace{
	class AlphaTweenerNew :public AlphaTweener{
	public:
		MobSprite* owner;
		AlphaTweenerNew(MobSprite* o, Visual* image, float alpha, float time)
			:owner(o),
			AlphaTweener(image, alpha, time)
		{

		}
	protected:
		virtual void onComplete()
		{
			owner->killAndErase();
			parent->erase(this);
		}
	};
}
void MobSprite::onComplete(Animation* anim)
{
	CharSprite::onComplete(anim);

	if (anim == die) 
	{
		parent->add(new AlphaTweenerNew(this, this, 0, FADE_TIME));
	}
}

namespace{
	class ScaleTweenerNew :public ScaleTweener{
	public:
		MobSprite* ms;
		ScaleTweenerNew(MobSprite* m, Visual* visual, PointF scale, float time)
			:ScaleTweener(visual, scale, time)
			,ms(m){}

	protected:
		virtual void onComplete()
		{
			ms->killAndErase();
			parent->erase(this);
		}
		virtual void updateValues(float progress)
		{
			ScaleTweener::updateValues(progress);
			ms->am = 1 - progress;
		}
	};
}
void MobSprite::fall()
{
	GameMath::PointFSet(&origin, width / 2, height - DungeonTilemap::SIZE / 2);
	//origin.set();
	angularSpeed = Random::Int(2) == 0 ? -720 : 720;

	parent->add(new ScaleTweenerNew(this, this, PointF(0, 0), FALL_TIME));
}

RatSprite::RatSprite()
{
	texture(Assets::RAT);

	TextureFilm* frames = new TextureFilm(tex, 16, 15);

	idle = new Animation(2, true);
	int arry1[] = { 0, 0, 0, 1 };
	idle->Frames(frames, std::vector<int>(arry1, arry1+sizeof(arry1) / sizeof(int)));

	run = new Animation(10, true);
	int arry2[] = { 6, 7, 8, 9, 10 };
	run->Frames(frames, std::vector<int>(arry2, arry2 + sizeof(arry2) / sizeof(int)));

	attack = new Animation(15, false);
	int arry3[] = { 2, 3, 4, 5, 0 };
	attack->Frames(frames, std::vector<int>(arry3, arry3 + sizeof(arry3) / sizeof(int)));

	die = new Animation(10, false);
	int arry4[] = { 11, 12, 13, 14 };
	die->Frames(frames, std::vector<int>(arry4, arry4 + sizeof(arry4) / sizeof(int)));

	play(idle);

	delete frames;
}

GnollSprite::GnollSprite()
{
	texture(Assets::GNOLL);

	TextureFilm* frames = new TextureFilm(tex, 12, 15);

	idle = new Animation(2, true);
	int arry1[] = { 0, 0, 0, 1, 0, 0, 1, 1 };
	idle->Frames(frames, arry1, sizeof(arry1)/sizeof(int));

	run = new Animation(12, true);
	int arry2[] = { 4, 5, 6, 7 };
	run->Frames(frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(12, false);
	int arry3[] = { 2, 3, 0 };
	attack->Frames(frames, arry3, sizeof(arry3) / sizeof(int));

	die = new Animation(12, false);
	int arry4[] = { 8, 9, 10 };
	die->Frames(frames, arry4, sizeof(arry4) / sizeof(int));

	play(idle);

	delete frames;
}

CrabSprite::CrabSprite()
{
	texture(Assets::CRAB);

	TextureFilm* frames = new TextureFilm(tex, 16);

	idle = new Animation(5, true);
	int arry1[] = { 0, 1, 0, 2 };
	idle->Frames(frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(15, true);
	int arry2[] = { 3, 4, 5, 6 };
	run->Frames(frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(12, false);
	int arry3[] = { 7, 8, 9 };
	attack->Frames(frames, arry3, sizeof(arry3) / sizeof(int));

	die = new Animation(12, false);
	int arry4[] = { 10, 11, 12, 13 };
	die->Frames(frames, arry4, sizeof(arry4) / sizeof(int));

	play(idle);

	delete frames;
}

SwarmSprite::SwarmSprite()
{
	texture(Assets::SWARM);

	TextureFilm* frames = new TextureFilm(tex, 16, 16);

	idle = new Animation(15, true);
	int arry1[] = { 0, 1, 2, 3, 4, 5 };
	idle->Frames(frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(15, true);
	int arry2[] = { 0, 1, 2, 3, 4, 5 };
	run->Frames(frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(20, false);
	int arry3[] = { 6, 7, 8, 9 };
	attack->Frames(frames, arry3, sizeof(arry3) / sizeof(int));

	die = new Animation(15, false);
	int arry4[] = { 10, 11, 12, 13, 14 };
	die->Frames(frames, arry4, sizeof(arry4) / sizeof(int));

	play(idle);

	delete frames;
}

SkeletonSprite::SkeletonSprite()
{
	texture(Assets::SKELETON);

	TextureFilm* frames = new TextureFilm(tex, 12, 15);

	idle = new Animation(12, true);
	int arry1[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3 };
	idle->Frames(frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(15, true);
	int arry2[] = { 4, 5, 6, 7, 8, 9 };
	run->Frames(frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(15, false);
	int arry3[] = { 14, 15, 16 };
	attack->Frames(frames, arry3, sizeof(arry3) / sizeof(int));

	die = new Animation(12, false);
	int arry4[] = { 10, 11, 12, 13 };
	die->Frames(frames, arry3, sizeof(arry3) / sizeof(int));

	play(idle);

	delete frames;
}

void SkeletonSprite::Die()
{
	MobSprite::Die();
	if (Dungeon::visible[ch->pos]) 
	{
		emitter()->burst(Speck::factory(Speck::BONE), 6);
	}
}

ThiefSprite::ThiefSprite()
{
	texture(Assets::THIEF);
	TextureFilm* film = new TextureFilm(tex, 12, 13);

	idle = new Animation(1, true);
	int arry1[] = { 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	idle->Frames(film, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(15, true);
	int arry2[] = { 0, 0, 2, 3, 3, 4 };
	run->Frames(film, arry2, sizeof(arry2) / sizeof(int));

	die = new Animation(10, false);
	int arry3[] = { 5, 6, 7, 8, 9 };
	die->Frames(film, arry3, sizeof(arry3) / sizeof(int));

	attack = new Animation(12, false);
	int arry4[] = { 10, 11, 12, 0 };
	attack->Frames(film, arry4, sizeof(arry4) / sizeof(int));

	Idle();

	delete film;
}

GooSprite::GooSprite()
{
	texture(Assets::GOO);

	TextureFilm* frames = new TextureFilm(tex, 20, 14);

	idle = new Animation(10, true);
	int arry1[] = { 0, 1 };
	idle->Frames(frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(10, true);
	int arry2[] = { 0, 1 };
	run->Frames(frames, arry2, sizeof(arry2) / sizeof(int));

	pump = new Animation(20, true);
	int arry3[] = { 0, 1 };
	pump->Frames(frames, arry3, sizeof(arry3) / sizeof(int));

	jump = new Animation(1, true);
	int arry4[] = { 6 };
	jump->Frames(frames, arry4, sizeof(arry4) / sizeof(int));

	attack = new Animation(10, false);
	int arry5[] = { 5, 0, 6 };
	attack->Frames(frames, arry5, sizeof(arry5) / sizeof(int));

	die = new Animation(10, false);
	int arry6[] = { 2, 3, 4 };
	die->Frames(frames, arry6, sizeof(arry6) / sizeof(int));

	MobSprite::play(idle);

	delete frames;
}

void GooSprite::play(Animation* anim, bool force)
{
	MobSprite::play(anim, force);

	if (anim == pump) 
	{
		spray = centerEmitter();
		spray->pour(GooParticle::FACTORY, 0.04f);
	}
	else if (spray != NULL) 
	{
		spray->on = false;
		spray = NULL;
	}
}

namespace{
	class FactoryNew :public Emitter::Factory{
	public:
		virtual void emit(Emitter* emitter, int index, float x, float y)
		{
			GooSprite::GooParticle* gp = (GooSprite::GooParticle*)emitter->recycle("GooParticle");
			if (gp == NULL)
			{
				gp = new GooSprite::GooParticle();
				emitter->add(gp);
			}
			gp->reset(x, y);
		}
	};
}
Emitter::Factory* GooSprite::GooParticle::FACTORY = new FactoryNew();

GooSprite::GooParticle::GooParticle()
{
	color(0x000000);
	_lifespan = 0.3f;

	GameMath::PointFSet(&acc, 0, +50);
	//acc.set(0, +50);
}

void GooSprite::GooParticle::reset(float x, float y)
{
	revive();

	this->x = x;
	this->y = y;

	_left = _lifespan;

	_sizef = 4;

	GameMath::PointFPolar(&speed, -Random::Float(GameMath::POINTF_PI), Random::Float(32, 48));
	//speed.polar(-Random.Float(PointF.PI), Random.Float(32, 48));
}

void GooSprite::GooParticle::update()
{
	Shrinking::update();
	float p = _left / _lifespan;
	am = p > 0.5f ? (1 - p) * 2.0f : 1;
}

ShamanSprite::ShamanSprite()
{
	texture(Assets::SHAMAN);

	TextureFilm* frames = new TextureFilm(tex, 12, 15);

	idle = new Animation(2, true);
	int arry1[] = { 0, 0, 0, 1, 0, 0, 1, 1 };
	idle->Frames(frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(12, true);
	int arry2[] = { 4, 5, 6, 7 };
	run->Frames(frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(12, false);
	int arry3[] = { 2, 3, 0 };
	attack->Frames(frames, arry3, sizeof(arry3) / sizeof(int));

	zap = attack->clone();

	die = new Animation(12, false);
	int arry4[] = { 8, 9, 10 };
	die->Frames(frames, arry4, sizeof(arry4) / sizeof(int));

	play(idle);

	delete frames;
}

void ShamanSprite::Zap(int pos)
{
	points[0] = ch->pos;
	points[1] = pos;
	std::vector<int> p(points, points+2);
	parent->add(new Lightning(p, 2, (Shaman*)ch));

	turnTo(ch->pos, pos);
	play(zap);
}

BatSprite::BatSprite()
{
	texture(Assets::BAT);

	TextureFilm* frames = new TextureFilm(tex, 15, 15);

	idle = new Animation(8, true);
	int arry1[] = { 0, 1 };
	idle->Frames(frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(12, true);
	int arry2[] = { 0, 1 };
	run->Frames(frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(12, false);
	int arry3[] = { 2, 3, 0, 1 };
	attack->Frames(frames, arry3, sizeof(arry3) / sizeof(int));

	die = new Animation(12, false);
	int arry4[] = { 4, 5, 6 };
	die->Frames(frames, arry4, sizeof(arry4) / sizeof(int));

	play(idle);

	delete frames;
}

BruteSprite::BruteSprite()
{
	texture(Assets::BRUTE);

	TextureFilm* frames = new TextureFilm(tex, 12, 16);

	idle = new Animation(2, true);
	int arry1[] = { 0, 0, 0, 1, 0, 0, 1, 1 };
	idle->Frames(frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(12, true);
	int arry2[] = { 4, 5, 6, 7 };
	run->Frames(frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(12, false);
	int arry3[] = { 2, 3, 0 };
	attack->Frames(frames, arry3, sizeof(arry3) / sizeof(int));

	die = new Animation(12, false);
	int arry4[] = { 8, 9, 10 };
	die->Frames(frames, arry4, sizeof(arry4) / sizeof(int));

	play(idle);

	delete frames;
}

TenguSprite::TenguSprite()
{
	texture(Assets::TENGU);

	TextureFilm* frames = new TextureFilm(tex, 14, 16);

	idle = new Animation(2, true);
	int arry1[] = { 0, 0, 0, 1 };
	idle->Frames(frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(15, false);
	int arry2[] = { 2, 3, 4, 5, 0 };
	run->Frames(frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(15, false);
	int arry3[] = { 6, 7, 7, 0 };
	attack->Frames(frames, arry3, sizeof(arry3) / sizeof(int));

	cast = attack->clone();

	die = new Animation(8, false);
	int arry4[] = { 8, 9, 10, 10, 10, 10, 10, 10 };
	die->Frames(frames, arry4, sizeof(arry4) / sizeof(int));

	play(run->clone());

	delete frames;
}

void TenguSprite::move(int from, int to)
{
	place(to);

	play(run);
	turnTo(from, to);

	isMoving = true;

	if (Level::water[to]) 
	{
		GameScene::ripple(to);
	}

	ch->onMotionComplete();
}

void TenguSprite::Attack(int cell)
{
	if (!Level::adjacent(cell, ch->pos)) 
	{
		//((MissileSprite)parent.recycle(MissileSprite.class)).
		//	reset(ch->pos, cell, new Shuriken(), new Callback(){
		//	@Override
		//	public void call() {
		//		ch.onAttackComplete();
		//	}
		//});

		play(cast);
		turnTo(ch->pos, cell);
	}
	else 
	{
		MobSprite::Attack(cell);
	}
}

void TenguSprite::onComplete(Animation* anim)
{
	if (anim == run) 
	{
		isMoving = false;
		Idle();
	}
	else 
	{
		MobSprite::onComplete(anim);
	}
}

SpinnerSprite::SpinnerSprite()
{
	texture(Assets::SPINNER);

	TextureFilm frames(tex, 16, 16);

	idle = new Animation(10, true);
	int arry1[] = { 0, 0, 0, 0, 0, 1, 0, 1 };
	idle->Frames(&frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(15, true);
	int arry2[] = { 0, 2, 0, 3 };
	run->Frames(&frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(12, false);
	int arry3[] = { 0, 4, 5, 0 };
	attack->Frames(&frames, arry3, sizeof(arry3) / sizeof(int));

	die = new Animation(12, false);
	int arry4[] = { 6, 7, 8, 9 };
	die->Frames(&frames, arry4, sizeof(arry4) / sizeof(int));

	play(idle);
}

ElementalSprite::ElementalSprite()
{
	texture(Assets::ELEMENTAL);

	TextureFilm frames(tex, 12, 14);

	idle = new Animation(10, true);
	int arry1[] = { 0, 1, 2 };
	idle->Frames(&frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(12, true);
	int arry2[] = { 0, 1, 3 };
	run->Frames(&frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(15, false);
	int arry3[] = { 4, 5, 6 };
	attack->Frames(&frames, arry3, sizeof(arry3) / sizeof(int));

	die = new Animation(15, false);
	int arry4[] = { 7, 8, 9, 10, 11, 12, 13, 12 };
	die->Frames(&frames, arry4, sizeof(arry4) / sizeof(int));

	play(idle);
}

MonkSprite::MonkSprite()
{
	texture(Assets::MONK);

	TextureFilm frames(tex, 15, 14);

	idle = new Animation(6, true);
	int arry1[] = { 1, 0, 1, 2 };
	idle->Frames(&frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(15, true);
	int arry2[] = { 11, 12, 13, 14, 15, 16 };
	run->Frames(&frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(12, false);
	int arry3[] = { 3, 4, 3, 4 };
	attack->Frames(&frames, arry3, sizeof(arry3) / sizeof(int));

	kick = new Animation(10, false);
	int arry4[] = { 5, 6, 5 };
	kick->Frames(&frames, arry4, sizeof(arry4) / sizeof(int));

	die = new Animation(15, false);
	int arry5[] = { 1, 7, 8, 8, 9, 10 };
	die->Frames(&frames, arry5, sizeof(arry5) / sizeof(int));

	play(idle);
}

void MonkSprite::Attack(int cell)
{
	MonkSprite::Attack(cell);
	if (Random::Float() < 0.5f) 
	{
		play(kick);
	}
}

void MonkSprite::onComplete(Animation* anim)
{
	MobSprite::onComplete(anim == kick ? attack : anim);
}

DM300Sprite::DM300Sprite()
{
	texture(Assets::DM300);

	TextureFilm frames(tex, 22, 20);

	idle = new Animation(10, true);
	int arry1[] = { 0, 1 };
	idle->Frames(&frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(10, true);
	int arry2[] = { 2, 3 };
	run->Frames(&frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(15, false);
	int arry3[] = { 4, 5, 6 };
	attack->Frames(&frames, arry3, sizeof(arry3) / sizeof(int));

	die = new Animation(20, false);
	int arry4[] = { 0, 7, 0, 7, 0, 7, 0, 7, 0, 7, 0, 7, 8 };
	die->Frames(&frames, arry4, sizeof(arry4) / sizeof(int));

	play(idle);
}

void DM300Sprite::onComplete(Animation* anim)
{
	MobSprite::onComplete(anim);

	if (anim == die) 
	{
		emitter()->burst(Speck::factory(Speck::WOOL), 15);
	}
}

WarlockSprite::WarlockSprite()
{
	texture(Assets::WARLOCK);

	TextureFilm frames(tex, 12, 15);

	idle = new Animation(2, true);
	int arry1[] = { 0, 0, 0, 1, 0, 0, 1, 1 };
	idle->Frames(&frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(15, true);
	int arry2[] = { 0, 2, 3, 4 };
	run->Frames(&frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(12, false);
	int arry3[] = { 0, 5, 6 };
	attack->Frames(&frames, arry3, sizeof(arry3) / sizeof(int));

	zap = attack->clone();

	die = new Animation(15, false);
	int arry4[] = { 0, 7, 8, 8, 9, 10 };
	die->Frames(&frames, arry4, sizeof(arry4) / sizeof(int));

	play(idle);
}

void WarlockSprite::Zap(int cell)
{
	turnTo(ch->pos, cell);
	play(zap);

	//MagicMissile.shadow(parent, ch.pos, cell,
	//	new Callback(){
	//	@Override
	//	public void call() {
	//		((Warlock)ch).onZapComplete();
	//	}
	//});
	//Sample.INSTANCE.play(Assets.SND_ZAP);
}

void WarlockSprite::onComplete(Animation* anim)
{
	if (anim == zap) 
	{
		Idle();
	}
	MobSprite::onComplete(anim);
}

GolemSprite::GolemSprite()
{
	texture(Assets::GOLEM);

	TextureFilm frames(tex, 16, 16);

	idle = new Animation(4, true);
	int arry1[] = { 0, 1 };
	idle->Frames(&frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(12, true);
	int arry2[] = { 2, 3, 4, 5 };
	run->Frames(&frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(10, false);
	int arry3[] = { 6, 7, 8 };
	attack->Frames(&frames, arry3, sizeof(arry3) / sizeof(int));

	die = new Animation(15, false);
	int arry4[] = { 9, 10, 11, 12, 13 };
	die->Frames(&frames, arry4, sizeof(arry4) / sizeof(int));

	play(idle);
}

void GolemSprite::onComplete(Animation* anim)
{
	//if (anim == die) {
	//	emitter().burst(ElmoParticle.FACTORY, 4);
	//}
	MobSprite::onComplete(anim);
}

SuccubusSprite::SuccubusSprite()
{
	texture(Assets::SUCCUBUS);

	TextureFilm frames(tex, 12, 15);

	idle = new Animation(8, true);
	int arry1[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 1 };
	idle->Frames(&frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(15, true);
	int arry2[] = { 3, 4, 5, 6, 7, 8 };
	run->Frames(&frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(12, false);
	int arry3[] = { 9, 10, 11 };
	attack->Frames(&frames, arry3, sizeof(arry3) / sizeof(int));

	die = new Animation(10, false);
	int arry4[] = { 12 };
	die->Frames(&frames, arry4, sizeof(arry4) / sizeof(int));

	play(idle);
}

void SuccubusSprite::Die()
{
	MobSprite::Die();
	emitter()->burst(Speck::factory(Speck::HEART), 6);
	//emitter()->burst(ShadowParticle.UP, 8);
}

KingSprite::KingSprite()
{
	texture(Assets::KING);

	TextureFilm frames(tex, 16, 16);

	idle = new Animation(12, true);
	int arry1[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2 };
	idle->Frames(&frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(15, true);
	int arry2[] = { 3, 4, 5, 6, 7, 8 };
	run->Frames(&frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(15, false);
	int arry3[] = { 9, 10, 11 };
	attack->Frames(&frames, arry3, sizeof(arry3) / sizeof(int));

	die = new Animation(8, false);
	int arry4[] = { 12, 13, 14, 15 };
	die->Frames(&frames, arry4, sizeof(arry4) / sizeof(int));

	play(idle);
}

UndeadSprite::UndeadSprite()
{
	texture(Assets::UNDEAD);

	TextureFilm frames(tex, 12, 16);

	idle = new Animation(12, true);
	int arry1[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3 };
	idle->Frames(&frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(15, true);
	int arry2[] = { 4, 5, 6, 7, 8, 9 };
	run->Frames(&frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(15, false);
	int arry3[] = { 14, 15, 16 };
	attack->Frames(&frames, arry3, sizeof(arry3) / sizeof(int));

	die = new Animation(12, false);
	int arry4[] = { 10, 11, 12, 13 };
	die->Frames(&frames, arry4, sizeof(arry4) / sizeof(int));

	play(idle);
}

void UndeadSprite::Die()
{
	MobSprite::Die();
	if (Dungeon::visible[ch->pos]) 
	{
		emitter()->burst(Speck::factory(Speck::BONE), 3);
	}
}

EyeSprite::EyeSprite()
{
	texture(Assets::EYE);

	TextureFilm frames(tex, 16, 18);

	idle = new Animation(8, true);
	int arry1[] = { 0, 1, 2 };
	idle->Frames(&frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(12, true);
	int arry2[] = { 5, 6 };
	run->Frames(&frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(8, false);
	int arry3[] = { 4, 3 };
	attack->Frames(&frames, arry3, sizeof(arry3) / sizeof(int));

	die = new Animation(8, false);
	int arry4[] = { 7, 8, 9 };
	die->Frames(&frames, arry4, sizeof(arry4) / sizeof(int));

	play(idle);
}

void EyeSprite::Attack(int pos)
{
	attackPos = pos;
	MobSprite::Attack(pos);
}

void EyeSprite::onComplete(Animation* anim)
{
	MobSprite::onComplete(anim);

	if (anim == attack) 
	{
		if (Dungeon::visible[ch->pos] || Dungeon::visible[attackPos]) 
		{
			//parent.add(new DeathRay(center(), DungeonTilemap.tileCenterToWorld(attackPos)));
		}
	}
}

ScorpioSprite::ScorpioSprite()
{
	texture(Assets::SCORPIO);

	TextureFilm frames(tex, 18, 17);

	idle = new Animation(12, true);
	int arry1[] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 2, 1, 2 };
	idle->Frames(&frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(8, true);
	int arry2[] = { 5, 5, 6, 6 };
	run->Frames(&frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(15, false);
	int arry3[] = { 0, 3, 4 };
	attack->Frames(&frames, arry3, sizeof(arry3) / sizeof(int));

	zap = attack->clone();

	die = new Animation(12, false);
	int arry4[] = { 0, 7, 8, 9, 10 };
	die->Frames(&frames, arry4, sizeof(arry4) / sizeof(int));

	play(idle);
}

void ScorpioSprite::Attack(int cell)
{
	if (!Level::adjacent(cell, ch->pos)) 
	{
		cellToAttack = cell;
		turnTo(ch->pos, cell);
		play(zap);
	}
	else 
	{
		MobSprite::Attack(cell);
	}
}

void ScorpioSprite::onComplete(Animation* anim)
{
	if (anim == zap) 
	{
		Idle();

		//((MissileSprite)parent.recycle(MissileSprite.class)).
		//	reset(ch.pos, cellToAttack, new Dart(), new Callback(){
		//	@Override
		//	public void call() {
		//		ch.onAttackComplete();
		//	}
		//});
	}
	else 
	{
		MobSprite::onComplete(anim);
	}
}

YogSprite::YogSprite()
{
	texture(Assets::YOG);

	TextureFilm frames(tex, 20, 19);

	idle = new Animation(10, true);
	int arry1[] = { 0, 1, 2, 2, 1, 0, 3, 4, 4, 3, 0, 5, 6, 6, 5 };
	idle->Frames(&frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(12, true);
	int arry2[] = { 0 };
	run->Frames(&frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(12, false);
	int arry3[] = { 0 };
	attack->Frames(&frames, arry3, sizeof(arry3) / sizeof(int));

	die = new Animation(10, false);
	int arry4[] = { 0, 7, 8, 9 };
	die->Frames(&frames, arry4, sizeof(arry4) / sizeof(int));

	play(idle);
}

void YogSprite::Die()
{
	MobSprite::Die();

	//Splash.at(center(), blood(), 12);
}

const float RottingFistSprite::FALL_SPEED = 64;

RottingFistSprite::RottingFistSprite()
{
	texture(Assets::ROTTING);

	TextureFilm frames(tex, 24, 17);

	idle = new Animation(2, true);
	int arry1[] = { 0, 0, 1 };
	idle->Frames(&frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(3, true);
	int arry2[] = { 0, 1 };
	run->Frames(&frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(2, false);
	int arry3[] = { 0 };
	attack->Frames(&frames, arry3, sizeof(arry3) / sizeof(int));

	die = new Animation(10, false);
	int arry4[] = { 0, 2, 3, 4 };
	die->Frames(&frames, arry4, sizeof(arry4) / sizeof(int));

	play(idle);
}

void RottingFistSprite::Attack(int cell)
{
	MobSprite::Attack(cell);

	GameMath::PointFSet(&speed, 0, -FALL_SPEED);
	//speed.set(0, -FALL_SPEED);
	GameMath::PointFSet(&acc, 0, FALL_SPEED * 4);
	//acc.set(0, FALL_SPEED * 4);
}

void RottingFistSprite::onComplete(Animation* anim)
{
	MobSprite::onComplete(anim);
	if (anim == attack) 
	{
		GameMath::PointFSet(&speed, 0);
		//speed.set(0);
		GameMath::PointFSet(&acc, 0);
		//acc.set(0);
		place(ch->pos);

		Camera::mainCamera->shake(4, 0.2f);
	}
}

BurningFistSprite::BurningFistSprite()
{
	texture(Assets::BURNING);

	TextureFilm frames(tex, 24, 17);

	idle = new Animation(2, true);
	int arry1[] = { 0, 0, 1 };
	idle->Frames(&frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(3, true);
	int arry2[] = { 0, 1 };
	run->Frames(&frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(8, false);
	int arry3[] = { 0, 5, 6 };
	attack->Frames(&frames, arry3, sizeof(arry3) / sizeof(int));

	die = new Animation(10, false);
	int arry4[] = { 0, 2, 3, 4 };
	die->Frames(&frames, arry4, sizeof(arry4) / sizeof(int));

	play(idle);
}

void BurningFistSprite::Attack(int cell)
{
	posToShoot = cell;
	MobSprite::Attack(cell);
}

void BurningFistSprite::onComplete(Animation* anim)
{
	if (anim == attack) 
	{
		//Sample.INSTANCE.play(Assets.SND_ZAP);
		//MagicMissile.shadow(parent, ch.pos, posToShoot,
		//	new Callback(){
		//	@Override
		//	public void call() {
		//		ch.onAttackComplete();
		//	}
		//});

		Idle();
	}
	else 
	{
		MobSprite::onComplete(anim);
	}
}

LarvaSprite::LarvaSprite()
{
	texture(Assets::LARVA);

	TextureFilm frames(tex, 12, 8);

	idle = new Animation(5, true);
	int arry1[] = { 4, 4, 4, 4, 4, 5, 5 };
	idle->Frames(&frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(12, true);
	int arry2[] = { 0, 1, 2, 3 };
	run->Frames(&frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(15, false);
	int arry3[] = { 6, 5, 7 };
	attack->Frames(&frames, arry3, sizeof(arry3) / sizeof(int));

	die = new Animation(10, false);
	int arry4[] = { 8 };
	die->Frames(&frames, arry4, sizeof(arry4) / sizeof(int));

	play(idle);
}

void LarvaSprite::Die()
{
	//Splash.at(center(), blood(), 10);
	MobSprite::Die();
}

SheepSprite::SheepSprite()
{
	texture(Assets::SHEEP);

	TextureFilm frames(tex, 16, 15);

	idle = new Animation(8, true);
	int arry1[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 0 };
	idle->Frames(&frames, arry1, sizeof(arry1) / sizeof(int));

	run = idle->clone();
	attack = idle->clone();

	die = new Animation(20, false);
	int arry2[] = { 0 };
	die->Frames(&frames, arry2, sizeof(arry2) / sizeof(int));

	play(idle);
	curFrame = Random::Int(curAnim->frames.size());
}

MimicSprite::MimicSprite()
{
	texture(Assets::MIMIC);

	TextureFilm frames(tex, 16, 16);

	idle = new Animation(5, true);
	int arry1[] = { 0, 0, 0, 1, 1 };
	idle->Frames(&frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(10, true);
	int arry2[] = { 0, 1, 2, 3, 3, 2, 1 };
	run->Frames(&frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(10, false);
	int arry3[] = { 0, 4, 5, 6 };
	attack->Frames(&frames, arry3, sizeof(arry3) / sizeof(int));

	die = new Animation(5, false);
	int arry4[] = { 7, 8, 9 };
	die->Frames(&frames, arry4, sizeof(arry4) / sizeof(int));

	play(idle);
}

WraithSprite::WraithSprite()
{
	texture(Assets::WRAITH);

	TextureFilm frames(tex, 14, 15);

	idle = new Animation(5, true);
	int arry1[] = { 0, 1 };
	idle->Frames(&frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(10, true);
	int arry2[] = { 0, 1 };
	run->Frames(&frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(10, false);
	int arry3[] = { 0, 2, 3 };
	attack->Frames(&frames, arry3, sizeof(arry3) / sizeof(int));

	die = new Animation(8, false);
	int arry4[] = { 0, 4, 5, 6, 7 };
	die->Frames(&frames, arry4, sizeof(arry4) / sizeof(int));

	play(idle);
}

ShopkeeperSprite::ShopkeeperSprite()
{
	texture(Assets::KEEPER);
	TextureFilm film(tex, 14, 14);

	idle = new Animation(10, true);
	int arry1[] = { 1, 1, 1, 1, 1, 0, 0, 0, 0 };
	idle->Frames(&film, arry1, sizeof(arry1) / sizeof(int));

	die = new Animation(20, false);
	int arry2[] = { 0 };
	die->Frames(&film, arry2, sizeof(arry2) / sizeof(int));

	run = idle->clone();
	attack = idle->clone();

	Idle();
}

void ShopkeeperSprite::onComplete(Animation* anim)
{
	MobSprite::onComplete(anim);

	if (visible && anim == idle) {
		if (coin == NULL) {
			coin = new PixelParticle();
			parent->add(coin);
		}
		coin->reset(x + (flipHorizontal ? 0 : 13), y + 7, 0xFFFF00, 1, 0.5f);
		coin->speed.y = -40;
		coin->acc.y = +160;
	}
}

MirrorSprite::MirrorSprite()
{
	texture(Dungeon::hero->heroClass.spritesheet());
	updateArmor(0);
	Idle();
}

void MirrorSprite::link(Char* ch)
{
	MobSprite::link(ch);
	updateArmor(((MirrorImage*)ch)->tier);
}

void MirrorSprite::updateArmor(int tier)
{
	TextureFilm film (HeroSprite::Tiers(), tier, FRAME_WIDTH, FRAME_HEIGHT);

	idle = new Animation(1, true);
	int arry1[] = { 0, 0, 0, 1, 0, 0, 1, 1 };
	idle->Frames(&film, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(20, true);
	int arry2[] = { 2, 3, 4, 5, 6, 7 };
	run->Frames(&film, arry2, sizeof(arry2) / sizeof(int));

	die = new Animation(20, false);
	int arry3[] = { 0 };
	die->Frames(&film, arry3, sizeof(arry3) / sizeof(int));

	attack = new Animation(15, false);
	int arry4[] = { 13, 14, 15, 0 };
	attack->Frames(&film, arry4, sizeof(arry4) / sizeof(int));

	Idle();
}
