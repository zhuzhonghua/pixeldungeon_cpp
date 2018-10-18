#include "stdafx.h"

#include "visual.h"
#include "game.h"
#include "util.h"
#include "camera.h"

Visual::Visual(float x, float y, float width, float height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;

	scale = PointF(1, 1);	

	angle = 0;
	angularSpeed = 0;

	resetColor();
}

void Visual::resetColor()
{
	rm = gm = bm = am = 1;
	ra = ga = ba = aa = 0;
}

void Visual::updateMotion()
{
	float elapsed = Game::elapsed;

	float d = (GameMath::speed(speed.x, acc.x) - speed.x) / 2;
	speed.x += d;
	x += speed.x * elapsed;
	speed.x += d;

	d = (GameMath::speed(speed.y, acc.y) - speed.y) / 2;
	speed.y += d;
	y += speed.y * elapsed;
	speed.y += d;

	angle += angularSpeed * elapsed;
}

void Visual::update()
{
	updateMotion();
}

void Visual::draw()
{
	updateMatrix();
}

void Visual::updateMatrix()
{
	_mat = Matrix(1.0f);

	_mat = glm::translate(_mat, glm::vec3(x, y, 0));
	_mat = glm::translate(_mat, glm::vec3(origin.x, origin.y, 0));

	if (angle != 0)
	{
		//_mat = glm::rotate(_mat, angle);
		GameMath::MATRIXRotate(_mat, angle);
	}
	if (scale.x != 1 || scale.y != 1)
	{
		//_mat = glm::scale(Matrix(1.0f), glm::vec3(scale.x, scale.y, 0))*_mat;
		GameMath::MATRIXScale(_mat, scale.x, scale.y);
	}
	_mat = glm::translate(_mat, glm::vec3(-origin.x,-origin.y,0));
}

void Visual::lightness(float value)
{
	if (value < 0.5f) 
	{
		rm = gm = bm = value * 2.0f;
		ra = ga = ba = 0;
	}
	else 
	{
		rm = gm = bm = 2.0f - value * 2.0f;
		ra = ga = ba = value * 2.0f - 1.0f;
	}
}

void Visual::tint(float r, float g, float b, float strength)
{
	rm = gm = bm = 1.0f - strength;
	ra = r * strength;
	ga = g * strength;
	ba = b * strength;
}

void Visual::tint(int color, float strength)
{
	rm = gm = bm = 1.0f - strength;
	ra = ((color >> 16) & 0xFF) / 255.0f * strength;
	ga = ((color >> 8) & 0xFF) / 255.0f * strength;
	ba = (color & 0xFF) / 255.0f * strength;
}

void Visual::color(float r, float g, float b)
{
	rm = gm = bm = 0;
	ra = r;
	ga = g;
	ba = b;
}

void Visual::color(int c)
{
	color(((c >> 16) & 0xFF) / 255.0f, ((c >> 8) & 0xFF) / 255.0f, (c & 0xFF) / 255.0f);
}

void Visual::hardlight(float r, float g, float b) 
{
	ra = ga = ba = 0;
	rm = r;
	gm = g;
	bm = b;
}

void Visual::hardlight(int c) 
{
	hardlight((c >> 16) / 255.0f, ((c >> 8) & 0xFF) / 255.0f, (c & 0xFF) / 255.0f);
}


bool Visual::overlapsPoint(float x, float y) 
{
	return x >= this->x && x < this->x + width * scale.x && y >= this->y && y < this->y + height * scale.y;
}

bool Visual::overlapsScreenPoint(int x, int y) 
{
	Camera* c = camera();
	if (c != NULL) 
	{
		PointF p = c->screenToCamera(x, y);
		return overlapsPoint(p.x, p.y);
	}
	else 
	{
		return false;
	}
}

bool Visual::isVisible() 
{
	Camera* c = camera();
	float cx = c->scroll.x;
	float cy = c->scroll.y;
	float w = getWidth();
	float h = getHeight();
	return x + w >= cx && y + h >= cy && x < cx + c->width && y < cy + c->height;
}