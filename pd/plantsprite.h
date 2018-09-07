#pragma once

#include "image.h"

class TextureFilm;
class Plant;

class PlantSprite :public Image{
private:
	static const float DELAY;

	enum State {
		GROWING, NORMAL, WITHERING
	};
	State state;
	float time;

	static TextureFilm* frames;

	int pos;

public:
	PlantSprite();
	PlantSprite(int image);

	void reset(Plant* plant);
	void reset(int image);
	virtual void update();
	virtual void kill();
};