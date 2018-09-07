#pragma once

#include "emitter.h"

class Splash{
public:
	static void at(int cell, int color, int n);
	static void at(PointF p, int color, int n);
	static void at(PointF p, float dir, float cone, int color, int n);

private:
	class SplashFactory:public Emitter::Factory{
	public:
		int color;
		float dir;
		float cone;

		virtual void emit(Emitter* emitter, int index, float x, float y);
	};

	static SplashFactory* FACTORY;
};