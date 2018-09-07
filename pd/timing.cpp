#include "stdafx.h"
#include "timing.h"

FPSLimiter::FPSLimiter()
{

}

void FPSLimiter::init(float maxFPS)
{
	setMaxFPS(maxFPS);
}

void FPSLimiter::setMaxFPS(float maxFPS)
{
	_maxFPS = maxFPS;
}

void FPSLimiter::begin()
{
	_startTicks = SDL_GetTicks();
}

float FPSLimiter::end()
{
	calculateFPS();

	int frameTicks = SDL_GetTicks() - _startTicks;
	if (1000.0f / _maxFPS > frameTicks)
	{
		SDL_Delay(1000.0f / _maxFPS - frameTicks);
	}

	return _fps;
}

void FPSLimiter::calculateFPS()
{
	static const int NUM_SAMPLES = 100;
	static float frameTimes[NUM_SAMPLES];
	static int currentFrame = 0;

	static int preTicks = SDL_GetTicks();

	int curTicks = SDL_GetTicks();

	_frameTime = curTicks - preTicks;

	frameTimes[currentFrame++ % NUM_SAMPLES] = _frameTime;
	preTicks = curTicks;

	int count = 0;
	if (currentFrame < NUM_SAMPLES)
	{
		count = currentFrame;
	}
	else
	{
		count = NUM_SAMPLES;
	}

	int frameTimeAvg = 0;
	for (int i = 0; i < count; i++)
	{
		frameTimeAvg += frameTimes[i];
	}
	frameTimeAvg /= count;

	if (frameTimeAvg > 0)
	{
		_fps = 1000.0f / frameTimeAvg;
	}
	else
	{
		_fps = 60;
	}
}