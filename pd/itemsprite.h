#pragma once

#include "movieclip.h"

class Item;
class Heap;

class ItemSprite :public MovieClip{
public:
	class Glowing {
	public:
		static Glowing* WHITE;

		int color;
		float red;
		float green;
		float blue;
		float period;

		Glowing(int color);

		Glowing(int color, float period);
	};
public:
	static const int SIZE = 16;

private:
	static const float DROP_INTERVAL;

protected:
	static TextureFilm* film;

public:
	Heap* heap;

private:
	Glowing* glowing;
	float phase;
	bool glowUp;

	float dropInterval;

public:
	ItemSprite();
	ItemSprite(Item* item);
	ItemSprite(int image, Glowing* glowing);
	void originToCenter();

	void link();
	void link(Heap* heap);

	virtual void revive();

	PointF worldToCamera(int cell);
	void place(int p);
	void drop();
	void drop(int from);

	ItemSprite* view(int image, Glowing* glowing);

	virtual void update();

	static int pick(int index, int x, int y);
};