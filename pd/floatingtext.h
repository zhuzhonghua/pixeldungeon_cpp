#pragma once

#include "bitmaptext.h"

#include <map>
#include <vector>

class FloatingText :public BitmapText{
private:
	static const float LIFESPAN;
	static const float DISTANCE;

	float timeLeft;

	int key;

	float cameraZoom;

	static std::map<int, std::vector<FloatingText*>> stacks;

	static void push(FloatingText* txt, int key);
public:
	FloatingText();

	virtual void update();
	virtual void kill();
	virtual void destroy();

	void reset(float x, float y, const std::string& text, int color);

	static void show(float x, float y, const std::string& text, int color);
	static void show(float x, float y, int key, const std::string& text, int color);
};