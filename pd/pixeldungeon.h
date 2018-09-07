#pragma once

#include "game.h"

class PixelDungeon :public Game{
public:
	PixelDungeon();
	~PixelDungeon();

	static bool landscape();
	static bool scaleUp();
	static void switchNoFade(Scene* c);

	static bool immersed();
	static void scaleUp(bool value);
	static void zoom(int value);
	static int zoom();
	static void music(bool value);
	static bool music();
	static void soundFx(bool value);
	static bool soundFx();
	static void brightness(bool value);
	static bool brightness();
	static void donated(const std::string& value);
	static std::string donated();
	static void lastClass(int value);
	static int lastClass();
	static void challenges(int value);
	static int challenges();
	static void intro(bool value);
	static bool intro();
};