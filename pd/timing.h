#pragma once

class FPSLimiter{
public:
	FPSLimiter();

	void init(float maxFPS);
	void setMaxFPS(float maxFPS);
	
	void begin();

	float end();
private:
	void calculateFPS();

	float _frameTime;
	float _maxFPS;
	float _fps;
	unsigned int _startTicks;
};
