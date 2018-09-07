#pragma once

#include "movieclip.h"
#include "tweener.h"
#include "callback.h"
#include "emitter.h"

class TorchHalo;
class IceBlock;
class Char;
class EmoIcon;

class CharSprite :public MovieClip, public Tweener::Listener, public MovieClip::Listener{
public:
	static const int DEFAULT = 0xFFFFFF;
	static const int POSITIVE = 0x00FF00;
	static const int NEGATIVE = 0xFF0000;
	static const int WARNING = 0xFF8800;
	static const int NEUTRAL = 0xFFFF00;

private:
	static const float MOVE_INTERVAL;
	static const float FLASH_INTERVAL;

	Tweener* jumpTweener;
	Callback* jumpCallback;

	float flashTime;

	class JumpTweener :public Tweener{
	public:
		Visual* visual;

		PointF start;
		PointF end;

		float height;

		JumpTweener(Visual* visual, PointF pos, float height, float time)
			:Tweener(visual, time)
		{
			this->visual = visual;
			start = visual->point();
			end = pos;

			this->height = height;
		}
	protected:
		virtual void updateValues(float progress);
	};
public:
	enum State {
		BURNING, LEVITATING, INVISIBLE, PARALYSED, FROZEN, ILLUMINATED
	};

	Char* ch;
	bool isMoving;

	CharSprite();
protected:
	Animation* idle;
	Animation* run;
	Animation* attack;
	Animation* operate;
	Animation* zap;
	Animation* die;

	Callback* animCallback;
	Tweener* motion;

	Emitter* burning;
	Emitter* levitation;

	IceBlock* iceBlock;
	TorchHalo* halo;

	EmoIcon* emo;

	bool sleeping;

public:
	void link(Char* ch);
	virtual void place(int cell) { point(worldToCamera(cell)); }
	void turnTo(int from, int to);
	PointF worldToCamera(int cell);

	void showStatus(int color, const std::string& text, ...);
	void Idle();
	virtual void move(int from, int to);

	void interruptMotion();
	virtual void Attack(int cell);
	virtual void Attack(int cell, Callback* callback);
	void Operate(int cell);
	void Zap(int cell);
	void jump(int from, int to, Callback* callback);
	virtual void Die();
	Emitter* emitter();
	Emitter* centerEmitter();
	Emitter* bottomEmitter();

	void burst(int color, int n);
	void bloodBurstA(PointF from, int damage);

	virtual int blood() { return 0xFFBB0000; }
	void flash() { ra = ba = ga = 1.0f; flashTime = FLASH_INTERVAL; }

	void add(State state);
	void remove(State state);

	virtual void update();
	void showSleep();
	void hideSleep();
	void showAlert();
	void hideAlert();

	virtual void kill();
	virtual void onComplete(Tweener* tweener);
	virtual void onComplete(Animation* anim);
};