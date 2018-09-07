#pragma once

#include "image.h"

class CharSprite;
class EmoIcon :public Image{
protected:
	float maxSize = 2;
	float timeScale = 1;

	bool growing = true;

	CharSprite* owner;

public:
	EmoIcon(CharSprite* owner);

	virtual void update();	
};

class EmoIconSleep :public EmoIcon
{
public:
	EmoIconSleep(CharSprite* owner);
};

class EmoIconAlert :public EmoIcon{
public:
	EmoIconAlert(CharSprite* owner);
};