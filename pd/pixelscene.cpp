#include "stdafx.h"

#include "pixelscene.h"
#include "pixeldungeon.h"
#include "game.h"
#include "visual.h"
#include "bitmaptext.h"
#include "badgebanner.h"

float PixelScene::defaultZoom = 0;
float PixelScene::minZoom;
float PixelScene::maxZoom;

Camera* PixelScene::uiCamera;

bool PixelScene::noFade = false;

void PixelScene::chooseFont(float size, float zoom)
{
	scale /= zoom;
}

void PixelScene::showBadge(const Badges::Badge* badge)
{
	BadgeBanner* banner = BadgeBanner::show(badge->image);
	banner->cameraf = uiCamera;
	banner->x = align(banner->cameraf, (banner->cameraf->width - banner->width) / 2);
	banner->y = align(banner->cameraf, (banner->cameraf->height - banner->height) / 3);
	Game::scene()->add(banner);
}

float PixelScene::scale = 0.3f;

PixelScene::PixelScene()
{

}

PixelScene::~PixelScene()
{

}

void PixelScene::init()
{
	Scene::init();

	float minWidth, minHeight;
	if (PixelDungeon::landscape()) 
	{
		minWidth = MIN_WIDTH_L;
		minHeight = MIN_HEIGHT_L;
	}
	else 
	{
		minWidth = MIN_WIDTH_P;
		minHeight = MIN_HEIGHT_P;
	}

	defaultZoom = (int)std::ceil(Game::density * 2.5f);
	while ((Game::width / defaultZoom < minWidth ||
		Game::height / defaultZoom < minHeight
		) && defaultZoom > 1) 
	{
		defaultZoom--;
	}

	if (PixelDungeon::scaleUp()) 
	{
		while (
			Game::width / (defaultZoom + 1) >= minWidth &&
			Game::height / (defaultZoom + 1) >= minHeight) 
		{
			defaultZoom++;
		}
	}

	minZoom = 1;
	maxZoom = defaultZoom * 2;

	Camera::reset(new PixelCamera(defaultZoom));

	float uiZoom = defaultZoom;
	uiCamera = Camera::createFullscreen(uiZoom);
	Camera::add(uiCamera);
}

float PixelScene::align(Camera* camera, float pos) {
	return ((int)(pos * camera->zoomf)) / camera->zoomf;
}

// This one should be used for UI elements
float PixelScene::align(float pos) {
	return ((int)(pos * defaultZoom)) / defaultZoom;
}

void PixelScene::align(Visual* v) {
	Camera* c = v->camera();
	v->x = align(c, v->x);
	v->y = align(c, v->y);
}

void PixelScene::fadeIn()
{
	if (noFade) 
	{
		noFade = false;
	}
	else 
	{
		fadeIn(0xFF000000, false);
	}
}

void PixelScene::fadeIn(int color, bool light)
{
	add(new Fader(color, light));
}

PixelCamera::PixelCamera(float zoom):
Camera((int)(Game::width - std::ceil(Game::width / zoom) * zoom) / 2,
(int)(Game::height - std::ceil(Game::height / zoom) * zoom) / 2,
(int)std::ceil(Game::width / zoom),
(int)std::ceil(Game::height / zoom), zoom)
{

}

void PixelCamera::updateMatrix()
{
	float sx = PixelScene::align(this, scroll.x + _shakeX);
	float sy = PixelScene::align(this, scroll.y + _shakeY);

	mat[0][0] = +zoomf * _invW2;
	mat[1][1] = -zoomf * _invH2;

	mat[3][0] = -1 + x * _invW2 - sx * mat[0][0];
	mat[3][1] = +1 - y * _invH2 - sy * mat[1][1];
}

PixelScene::Fader::Fader(int color, bool light) 
:ColorBlock(uiCamera->width, uiCamera->height, color)
{
	this->_light = light;

	cameraf = uiCamera;

	alpha(1.0f);
	_time = Fader_FADE_TIME;
}

void PixelScene::Fader::update()
{
	ColorBlock::update();

	if ((_time -= Game::elapsed) <= 0) 
	{
		alpha(0.0f);
		parent->remove(this, true);
	}
	else 
	{
		alpha(_time / Fader_FADE_TIME);
	}
}

void PixelScene::Fader::draw()
{
	if (_light) 
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		ColorBlock::draw();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else 
	{
		ColorBlock::draw();
	}
}


BitmapText* PixelScene::createText(float size)
{
	return createText("", size);
}

BitmapText* PixelScene::createText(const std::string& text, float size)
{
	BitmapText* result = new BitmapText(text);

	float nscale = scale;
	if (size == 6)
	{
		nscale -= 0.15f;
	}

	GameMath::PointFSet(&result->scale, nscale);
	//result->scale.set(scale);

	return result;
}

BitmapText* PixelScene::createText(Uint16 ch, float size)
{
	BitmapText* result = new BitmapText(ch);

	float nscale = scale;
	if (size == 6)
	{
		nscale -= 0.1f;
	}

	GameMath::PointFSet(&result->scale, nscale);
	//result->scale.set(scale);

	return result;
}

BitmapTextMultiline* PixelScene::createMultiline(float size)
{
	return createMultiline("", size);
}

BitmapTextMultiline* PixelScene::createMultiline(const std::string& text, float size)
{
	BitmapTextMultiline* result = new BitmapTextMultiline(text);
	float nscale = scale;
	if (size == 6)
	{
		nscale -= 0.1f;
	}
	else if (size == 7)
	{
		nscale -= 0.07f;
	}
	GameMath::PointFSet(&result->scale, nscale);
	//result->scale.set(scale);
	return result;
}