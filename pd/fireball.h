#pragma once

#include "component.h"
#include "define.h"
#include "image.h"
#include "emitter.h"

const RectF Fireball_BLIGHT(0.0f, 0.0f, 0.25f, 1.0f);
const RectF Fireball_FLIGHT(0.25f, 0, 0.5f, 1);
const RectF Fireball_FLAME1(0.50f, 0, 0.75f, 1);
const RectF Fireball_FLAME2(0.75f, 0, 1.00f, 1);

class EmitterFactory1 :public Emitter::Factory{
public:
	virtual void emit(Emitter* emitter, int index, float x, float y);
};

class Fireball :public Component{
private:
	static const int COLOR = 0xFF66FF;

	Image* _bLight;
	Image* _fLight;
	Emitter* _emitter;
	Group* _sparks;

protected:
	virtual void createChildren();
	virtual void layout();
	virtual void update();
public:
	Fireball();
};

class Flame :public Image{
private:
	static float LIFESPAN;

	static float SPEED;
	static float ACC;

	float timeLeft;

public:
	Flame();
	void init();
	void reset();

	virtual void update();
};