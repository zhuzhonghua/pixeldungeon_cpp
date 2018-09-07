#include "stdafx.h"
#include "charsprite.h"
#include "torchhalo.h"
#include "util.h"
#include "char.h"
#include "level.h"
#include "dungeontilemap.h"
#include "floatingtext.h"
#include "postweener.h"
#include "gamescene.h"
#include "emoicon.h"
#include "splash.h"
#include "flameparticle.h"
#include "speck.h"
#include "iceblock.h"
#include "game.h"

const float CharSprite::MOVE_INTERVAL = 0.1f;
const float CharSprite::FLASH_INTERVAL = 0.05f;

CharSprite::CharSprite()
{
	flashTime = 0;
	isMoving = false;
	sleeping = false;

	listener = this;

	idle = NULL;
	run = NULL;
	attack = NULL;
	operate = NULL;
	zap = NULL;
	die = NULL;

	animCallback = NULL;
	motion = NULL;

	burning = NULL;
	levitation = NULL;

	iceBlock = NULL;
	halo = NULL;

	emo = NULL;
}

void CharSprite::link(Char* ch)
{
	this->ch = ch;
	ch->sprite = this;

	place(ch->pos);
	turnTo(ch->pos, Random::Int(Level::LENGTH));

	ch->updateSpriteState();
}

void CharSprite::turnTo(int from, int to)
{
	int fx = from % Level::WIDTH;
	int tx = to % Level::WIDTH;
	if (tx > fx) 
	{
		flipHorizontal = false;
	}
	else if (tx < fx) 
	{
		flipHorizontal = true;
	}
}

PointF CharSprite::worldToCamera(int cell)
{
	int csize = DungeonTilemap::SIZE;

	return PointF(
		((cell % Level::WIDTH) + 0.5f) * csize - width * 0.5f,
		((cell / Level::WIDTH) + 1.0f) * csize - height
		);
}

void CharSprite::showStatus(int color, const std::string& text, ...)
{
	if (visible) 
	{
		char buf[256] = { 0 };

		va_list args;
		va_start(args, text);
		vsprintf(buf, text.c_str(), args);
		va_end(args);

		if (ch != NULL) 
		{
			FloatingText::show(x + width * 0.5f, y, ch->pos, std::string(buf), color);
		}
		else 
		{
			FloatingText::show(x + width * 0.5f, y, std::string(buf), color);
		}
	}
}

void CharSprite::Idle()
{
	play(idle);
}

void CharSprite::move(int from, int to)
{
	play(run);

	motion = new PosTweener(this, worldToCamera(to), MOVE_INTERVAL);
	motion->listener = this;
	parent->add(motion);

	isMoving = true;

	turnTo(from, to);

	if (visible && Level::water[from] && !ch->flying) 
	{
		GameScene::ripple(from);
	}

	ch->onMotionComplete();
}

void CharSprite::interruptMotion()
{
	if (motion != NULL) 
	{
		onComplete(motion);
	}
}

void CharSprite::Attack(int cell)
{
	turnTo(ch->pos, cell);
	play(attack);
}

void CharSprite::Attack(int cell, Callback* callback)
{
	animCallback = callback;
	turnTo(ch->pos, cell);
	play(attack);
}

void CharSprite::Operate(int cell)
{
	turnTo(ch->pos, cell);
	play(operate);
}

void CharSprite::Zap(int cell)
{
	turnTo(ch->pos, cell);
	play(zap);
}

void CharSprite::jump(int from, int to, Callback* callback)
{
	jumpCallback = callback;

	int distance = Level::distance(from, to);
	jumpTweener = new JumpTweener(this, worldToCamera(to), distance * 4, distance * 0.1f);
	jumpTweener->listener = this;
	parent->add(jumpTweener);

	turnTo(from, to);
}

void CharSprite::Die()
{
	sleeping = false;
	play(die);

	if (emo != NULL) 
	{
		emo->killAndErase();
	}
}

Emitter* CharSprite::emitter()
{
	Emitter* emitter = GameScene::emitter();
	emitter->pos(this);
	return emitter;
}

Emitter* CharSprite::centerEmitter()
{
	Emitter* emitter = GameScene::emitter();
	emitter->pos(center());
	return emitter;
}

Emitter* CharSprite::bottomEmitter()
{
	Emitter* emitter = GameScene::emitter();
	emitter->pos(x, y + height, width, 0);
	return emitter;
}

void CharSprite::burst(int color, int n)
{
	if (visible) 
	{
		Splash::at(center(), color, n);
	}
}

void CharSprite::bloodBurstA(PointF from, int damage)
{
	if (visible) 
	{
		PointF c = center();
		int n = (int)std::min(9 * std::sqrt((double)damage / ch->HT), (double)9);

		Splash::at(c, GameMath::PointFAngle(from, c), 3.1415926f / 2, blood(), n);
	}
}

void CharSprite::add(State state)
{
	switch (state) 
	{
	case BURNING:
		burning = emitter();
		burning->pour(FlameParticle::FACTORY, 0.06f);
		if (visible) {
			//Sample.INSTANCE.play(Assets.SND_BURNING);
		}
		break;
	case LEVITATING:
		levitation = emitter();
		levitation->pour(Speck::factory(Speck::JET), 0.02f);
		break;
	case INVISIBLE:
		//PotionOfInvisibility.melt(ch);
		break;
	case PARALYSED:
		paused = true;
		break;
	case FROZEN:
		iceBlock = IceBlock::freeze(this);
		paused = true;
		break;
	case ILLUMINATED:
		GameScene::effect(halo = new TorchHalo(this));
		break;
	}
}

void CharSprite::remove(State state)
{
	switch (state) 
	{
	case BURNING:
		if (burning != NULL) 
		{
			burning->on = false;
			burning = NULL;
		}
		break;
	case LEVITATING:
		if (levitation != NULL) 
		{
			levitation->on = false;
			levitation = NULL;
		}
		break;
	case INVISIBLE:
		alpha(1.0f);
		break;
	case PARALYSED:
		paused = false;
		break;
	case FROZEN:
		if (iceBlock != NULL) 
		{
			iceBlock->melt();
			iceBlock = NULL;
		}
		paused = false;
		break;
	case ILLUMINATED:
		if (halo != NULL) 
		{
			halo->putOut();
		}
		break;
	}
}

void CharSprite::update()
{
	MovieClip::update();

	if (paused && listener != NULL) 
	{
		listener->onComplete(curAnim);
	}

	if (flashTime > 0 && (flashTime -= Game::elapsed) <= 0) 
	{
		resetColor();
	}

	if (burning != NULL) 
	{
		burning->visible = visible;
	}
	if (levitation != NULL) 
	{
		levitation->visible = visible;
	}
	if (iceBlock != NULL) 
	{
		iceBlock->visible = visible;
	}
	if (sleeping) 
	{
		showSleep();
	}
	else 
	{
		hideSleep();
	}
	if (emo != NULL) 
	{
		emo->visible = visible;
	}
}

void CharSprite::showSleep()
{
	if (dynamic_cast<EmoIconSleep*>(emo)) 
	{

	}
	else 
	{
		if (emo != NULL) 
		{
			emo->killAndErase();
		}
		emo = new EmoIconSleep(this);
	}
}

void CharSprite::hideSleep()
{
	if (dynamic_cast<EmoIconSleep*>(emo)) 
	{
		emo->killAndErase();
		emo = NULL;
	}
}

void CharSprite::showAlert()
{
	if (dynamic_cast<EmoIconAlert*>(emo)) 
	{

	}
	else {
		if (emo != NULL) 
		{
			emo->killAndErase();
		}
		emo = new EmoIconAlert(this);
	}
}

void CharSprite::hideAlert()
{
	if (dynamic_cast<EmoIconAlert*>(emo)) 
	{
		emo->killAndErase();
		emo = NULL;
	}
}

void CharSprite::kill()
{
	MovieClip::kill();

	if (emo != NULL) 
	{
		emo->killAndErase();
		emo = NULL;
	}
}

void CharSprite::onComplete(Tweener* tweener)
{
	if (tweener == jumpTweener) 
	{
		if (visible && Level::water[ch->pos] && !ch->flying) 
		{
			GameScene::ripple(ch->pos);
		}
		if (jumpCallback != NULL) 
		{
			jumpCallback->call();
		}

	}
	else if (tweener == motion) 
	{
		isMoving = false;

		motion->killAndErase();
		motion = NULL;
	}
}

void CharSprite::onComplete(Animation* anim)
{
	if (animCallback != NULL) 
	{
		animCallback->call();
		animCallback = NULL;
	}
	else 
	{
		if (anim == attack) 
		{
			Idle();
			ch->onAttackComplete();
		}
		else if (anim == operate) 
		{
			Idle();
			ch->onOperateComplete();
		}
	}
}

void CharSprite::JumpTweener::updateValues(float progress)
{
	PointF p = GameMath::PointFInter(start, end, progress);
	GameMath::PointFOffSet(&p, 0, -height * 4 * progress * (1 - progress));

	visual->point(p);
}