#pragma once

#include "window.h"

#include "bitmaptextmultiline.h"

class WndStory :public Window{
private:
	static const int WIDTH = 120;
	static const int MARGIN = 6;

	static const float bgR;
	static const float bgG;
	static const float bgB;

public:
	static const int ID_SEWERS = 0;
	static const int ID_PRISON = 1;
	static const int ID_CAVES = 2;
	static const int ID_METROPOLIS = 3;
	static const int ID_HALLS = 4;

	WndStory(const std::string& text);

	virtual void update();
	static void showChapter(int id);
private:
	static std::map<int, std::string> CHAPTERS;
	static void initChapters();

	BitmapTextMultiline* tf;
	float delay;
};