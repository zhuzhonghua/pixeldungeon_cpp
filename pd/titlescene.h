#pragma once

#include "pixelscene.h"
#include "button.h"
#include <string>
#include "bitmaptext.h"

class TitleScene : public PixelScene{
private:
	const std::string TXT_PLAY = "lang.play";
	const std::string TXT_HIGHSCORES = "lang.rank";
	const std::string TXT_BADGES = "lang.badge";
	const std::string TXT_ABOUT = "lang.about";

public:
	void init();

	void placeTorch(float x, float y);
};

#include "image.h"
class SignImage :public Image{
private:
	float time;
public:
	SignImage(Image* img);
	virtual void update();
	virtual void draw();
};

const float  DashboardItem_SIZE = 48.0f;
class DashboardItem:public Button{
public:
	DashboardItem(const std::string& text, int index);
protected:
	virtual void createChildren();
	virtual void layout();
	virtual void onTouchDown();
	virtual void onTouchUp();
private:
	static const int IMAGE_SIZE = 32;
	Image* image;
	BitmapText* label;
};