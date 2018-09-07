#pragma once

#include "scene.h"
#include "camera.h"
#include "colorblock.h"
#include "bitmaptext.h"
#include "bitmaptextmultiline.h"
#include "badge.h"

class Visual;
class PixelScene : public Scene{
public:
	// Minimum virtual display size for portrait orientation
	const float MIN_WIDTH_P = 128;
	const float MIN_HEIGHT_P = 224;

	// Minimum virtual display size for landscape orientation
	const float MIN_WIDTH_L = 224;
	const float MIN_HEIGHT_L = 160;

	static float defaultZoom;
	static float minZoom;
	static float maxZoom;

	static Camera* uiCamera;
public:
	PixelScene();
	~PixelScene();

	virtual void init();

	static float scale;

	static BitmapText* createText(float size);
	static BitmapText* createText(const std::string& text, float size);
	static BitmapText* createText(Uint16 ch, float size);

	static BitmapTextMultiline* createMultiline(float size);
	static BitmapTextMultiline* createMultiline(const std::string& text, float size);

	static float align(Camera* camera, float pos);
	// This one should be used for UI elements
	static float align(float pos);
	static void align(Visual* v);

	static bool noFade;

	class Fader :public ColorBlock{
	private:
		bool _light;
		float _time;

	public:
		Fader(int color, bool light);

		virtual void update();
		virtual void draw();
	};

	static void chooseFont(float size, float zoom);
	static void showBadge(const Badges::Badge* badge);
protected:
	void fadeIn();
	void fadeIn(int color, bool light);
};

const float Fader_FADE_TIME = 1.0f;

class PixelCamera:public Camera {
public:
	PixelCamera(float zoom);

	void updateMatrix();
};