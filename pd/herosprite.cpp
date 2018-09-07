#include "herosprite.h"
#include "camera.h"
#include "char.h"
#include "dungeon.h"
#include "texturecache.h"
#include "util.h"
#include "hero.h"

TextureFilm* HeroSprite::tiers;

HeroSprite::HeroSprite()
{
	animCallback = NULL;

	link(Dungeon::hero);

	texture(Dungeon::hero->heroClass.spritesheet());
	updateArmor();

	Idle();
}

void HeroSprite::updateArmor()
{
	TextureFilm* film = new TextureFilm(Tiers(), ((Hero*)ch)->tier(), FRAME_WIDTH, FRAME_HEIGHT);

	idle = new Animation(1, true);
	int fms[] = { 0, 0, 0, 1, 0, 0, 1, 1 };
	idle->Frames(film, std::vector<int>(fms, fms+sizeof(fms)/sizeof(int)));

	run = new Animation(RUN_FRAMERATE, true);
	int fms2[] = { 2, 3, 4, 5, 6, 7 };
	run->Frames(film, std::vector<int>(fms2, fms2 + sizeof(fms2) / sizeof(int)));

	die = new Animation(20, false);
	int fms3[] = { 8, 9, 10, 11, 12, 11 };
	die->Frames(film, std::vector<int>(fms3, fms3 + sizeof(fms3) / sizeof(int)));

	attack = new Animation(15, false);
	int fms4[] = { 13, 14, 15, 0 };
	attack->Frames(film, std::vector<int>(fms4, fms4 + sizeof(fms4) / sizeof(int)));

	zap = attack->clone();

	operate = new Animation(8, false);
	int fms5[] = { 16, 17, 16, 17 };
	operate->Frames(film, std::vector<int>(fms5, fms5 + sizeof(fms5) / sizeof(int)));

	fly = new Animation(1, true);
	int fms6[] = { 18 };
	fly->Frames(film, std::vector<int>(fms6, fms6 + sizeof(fms6) / sizeof(int)));

	read = new Animation(20, false);
	int fms7[] = { 19, 20, 20, 20, 20, 20, 20, 20, 20, 19 };
	read->Frames(film, std::vector<int>(fms7, fms7 + sizeof(fms7) / sizeof(int)));
}

void HeroSprite::place(int p)
{
	CharSprite::place(p);
	Camera::mainCamera->target = this;
}

void HeroSprite::move(int from, int to)
{
	CharSprite::move(from, to);
	if (ch->flying) 
	{
		play(fly);
	}
	Camera::mainCamera->target = this;
}

void HeroSprite::jump(int from, int to, Callback* callback)
{
	CharSprite::jump(from, to, callback);
	play(fly);
}

TextureFilm* HeroSprite::Tiers()
{
	if (tiers == NULL) 
	{
		SmartTexture* texture = TextureCache::getTexture(Assets::ROGUE);
		tiers = new TextureFilm(texture, texture->width, FRAME_HEIGHT);
	}

	return tiers;
}

namespace{
	class CallbackNew :public Callback{
	public:
		HeroSprite* hs;
		CallbackNew(HeroSprite* hsp) :hs(hsp){}

		virtual void call()
		{
			hs->Idle();
			hs->ch->onOperateComplete();
		}
	};
}
void HeroSprite::Read()
{
	if (animCallback)
	{
		delete animCallback;
		animCallback = NULL;
	}
	animCallback = new CallbackNew(this);
	play(read);
}

void HeroSprite::update()
{
	sleeping = ((Hero*)ch)->restoreHealth;

	CharSprite::update();
}

bool HeroSprite::sprint(bool on)
{
	run->delay = on ? 0.625f / RUN_FRAMERATE : 1.0f / RUN_FRAMERATE;
	return on;
}

Image* HeroSprite::avatar(HeroClass cl, int armorTier)
{
	RectF patch = Tiers()->get(armorTier);
	Image* avatar = new Image(cl.spritesheet());
	RectF frame = avatar->tex->uvRect(1, 0, FRAME_WIDTH, FRAME_HEIGHT);

	float patLeft = GameMath::RECTFLeft(patch);
	float patTop = GameMath::RECTFTop(patch);

	GameMath::RECTFOffset(frame, patLeft, patTop);

	//frame.offset(patch.left, patch.top);
	avatar->frame(frame);

	return avatar;
}
