#pragma once

#include "pixelscene.h"
#include "pthread/pthread.h"

class InterlevelScene :public PixelScene{
public:
	static const float TIME_TO_FADE;

	static const std::string TXT_DESCENDING;
	static const std::string TXT_ASCENDING;
	static const std::string TXT_LOADING;
	static const std::string TXT_RESURRECTING;
	static const std::string TXT_RETURNING;
	static const std::string TXT_FALLING;

	static const std::string ERR_FILE_NOT_FOUND;
	static const std::string ERR_GENERIC;

public:
	enum Phase {
		FADE_IN, STATIC, FADE_OUT
	};

	Phase phase;
	float timeLeft;

	BitmapText* message;
	std::string error;
	bool threadEndFlag;

	bool isThreadAlive();
	void descend();
	void fall();
	void ascend();
	void returnTo();
	void restore();
	void resurrect();
public:
	enum Mode {
		DESCEND, ASCEND, CONTINUE, RESURRECT, RETURN, FALL, NONE
	};

	static Mode mode;

	static int returnDepth;
	static int returnPos;

	static bool noStory;

	static bool fallIntoPit;

	virtual void init();
	virtual void update();
};