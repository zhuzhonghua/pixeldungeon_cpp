#include "movieclip.h"

#include "game.h"

MovieClip::Animation::Animation(int fps, bool looped)
{
	this->delay = 1.0f / fps;
	this->looped = looped;
}

MovieClip::Animation* MovieClip::Animation::Frames(const std::vector<RectF>& frames)
{
	this->frames = frames;
	return this;
}

MovieClip::Animation* MovieClip::Animation::Frames(TextureFilm* film, std::vector<int> frames)
{
	this->frames.resize(frames.size());
	for (int i = 0; i < frames.size(); i++) 
	{
		this->frames[i] = film->get(frames[i]);
	}
	return this;
}

MovieClip::Animation* MovieClip::Animation::Frames(TextureFilm* film, int frames[], int size)
{
	this->frames.resize(size);
	for (int i = 0; i < size; i++)
	{
		this->frames[i] = film->get(frames[i]);
	}
	return this;
}

MovieClip::Animation* MovieClip::Animation::clone()
{
	MovieClip::Animation* ma = new MovieClip::Animation(std::round(1 / delay), looped);
	return ma->Frames(frames);
}

MovieClip::MovieClip()
:Image()
{
	init();
}

MovieClip::MovieClip(const std::string& tx)
:Image(tx)
{
	init();
}

void MovieClip::init()
{
	paused = false;
	curAnim = NULL;
	curFrame = 0;
	frameTimer = 0;
	finished = false;
}

void MovieClip::update()
{
	Image::update();
	if (!paused)
	{
		updateAnimation();
	}
}

void MovieClip::updateAnimation()
{
	if (curAnim != NULL && curAnim->delay > 0 && (curAnim->looped || !finished)) 
	{
		int lastFrame = curFrame;

		frameTimer += Game::elapsed;
		while (frameTimer > curAnim->delay) 
		{
			frameTimer -= curAnim->delay;
			if (curFrame == curAnim->frames.size() - 1) 
			{
				if (curAnim->looped) 
				{
					curFrame = 0;
				}
				finished = true;
				if (listener != NULL) 
				{
					listener->onComplete(curAnim);
					// This check can probably be removed
					if (curAnim == NULL) 
					{
						return;
					}
				}
			}
			else 
			{
				curFrame++;
			}
		}

		if (curFrame != lastFrame) 
		{
			frame(curAnim->frames[curFrame]);
		}
	}
}

void MovieClip::play(Animation* anim)
{
	play(anim, false);
}

void MovieClip::play(Animation* anim, bool force)
{
	if (!force && (curAnim != NULL) && (curAnim == anim) && (curAnim->looped || !finished)) 
	{
		return;
	}

	curAnim = anim;
	curFrame = 0;
	finished = false;

	frameTimer = 0;

	if (anim != NULL) 
	{
		frame(anim->frames[curFrame]);
	}
}