#include "stdafx.h"

#include "pixeldungeon.h"
#include "pixelscene.h"
#include "preferences.h"
#include "titlescene.h"
#include "gamescene.h"

PixelDungeon::PixelDungeon()
{

}

PixelDungeon::~PixelDungeon()
{

}

bool PixelDungeon::landscape()
{
	return width > height;
}

bool PixelDungeon::scaleUp()
{
	return Preferences::get()->getBool(Preferences::KEY_SCALE_UP, true);
}

void PixelDungeon::switchNoFade(Scene* c)
{
	PixelScene::noFade = true;
	switchScene(c);
}

bool PixelDungeon::immersed()
{
	return Preferences::get()->getBool(Preferences::KEY_IMMERSIVE, false);
}

void PixelDungeon::scaleUp(bool value)
{
	Preferences::get()->put(Preferences::KEY_SCALE_UP, value);
	switchScene(new TitleScene());
}

void PixelDungeon::zoom(int value)
{
	Preferences::get()->put(Preferences::KEY_ZOOM, value);
}

int PixelDungeon::zoom()
{
	return Preferences::get()->getInt(Preferences::KEY_ZOOM, 0);
}

void PixelDungeon::music(bool value)
{
	//Music.INSTANCE.enable(value);
	Preferences::get()->put(Preferences::KEY_MUSIC, value);
}

bool PixelDungeon::music()
{
	return Preferences::get()->getBool(Preferences::KEY_MUSIC, true);
}

void PixelDungeon::soundFx(bool value)
{
	//Sample.INSTANCE.enable(value);
	Preferences::get()->put(Preferences::KEY_SOUND_FX, value);
}

bool PixelDungeon::soundFx()
{
	return Preferences::get()->getBool(Preferences::KEY_SOUND_FX, true);
}
void PixelDungeon::brightness(bool value)
{
	Preferences::get()->put(Preferences::KEY_BRIGHTNESS, value);
	if (dynamic_cast<GameScene*>(scene())) 
	{
		((GameScene*)scene())->brightness(value);
	}
}
bool PixelDungeon::brightness()
{
	return Preferences::get()->getBool(Preferences::KEY_BRIGHTNESS, false);
}
void PixelDungeon::donated(const std::string& value)
{
	Preferences::get()->put(Preferences::KEY_DONATED, value);
}
std::string PixelDungeon::donated()
{
	return Preferences::get()->getString(Preferences::KEY_DONATED, "");
}
void PixelDungeon::lastClass(int value)
{
	Preferences::get()->put(Preferences::KEY_LAST_CLASS, value);
}
int PixelDungeon::lastClass()
{
	return Preferences::get()->getInt(Preferences::KEY_LAST_CLASS, 0);
}
void PixelDungeon::challenges(int value)
{
	Preferences::get()->put(Preferences::KEY_CHALLENGES, value);
}
int PixelDungeon::challenges()
{
	return Preferences::get()->getInt(Preferences::KEY_CHALLENGES, 0);
}
void PixelDungeon::intro(bool value)
{
	Preferences::get()->put(Preferences::KEY_INTRO, value);
}
bool PixelDungeon::intro()
{
	return Preferences::get()->getBool(Preferences::KEY_INTRO, true);
}