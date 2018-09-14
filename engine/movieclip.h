#pragma once

#include "image.h"
#include "texturefilm.h"

class MovieClip :public Image{
public:
	class Animation{
	public:
		float delay;
		std::vector<RectF> frames;
		bool looped;

		Animation(int fps, bool looped);
		Animation* Frames(const std::vector<RectF>& frames);
		Animation* Frames(TextureFilm* film, std::vector<int> frames);
		Animation* Frames(TextureFilm* film, int frames[], int size);
		Animation* clone();
	};

	class Listener
	{
	public:
		virtual void onComplete(Animation* anim) = 0;
	};

protected:
	Animation* curAnim;
	int curFrame;
	float frameTimer;
	bool finished;

	void init();
	void updateAnimation();
public:
	bool paused;

	Listener* listener;

	MovieClip();
	MovieClip(const std::string& tx);

	virtual void update();

	void play(Animation* anim);
	virtual void play(Animation* anim, bool force);
};