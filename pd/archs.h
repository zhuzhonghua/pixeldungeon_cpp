#pragma once

#include "component.h"

class SkinnedBlock;

class Archs :public Component
{
private:
	static const int SCROLL_SPEED = 20;

	SkinnedBlock* _arcsBg;
	SkinnedBlock* _arcsFg;

	static float _offsB;
	static float _offsF;

public:
	bool reversed;

	void update();

	Archs();
protected:
	void createChildren();
	void layout();
};