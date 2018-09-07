#include "stdafx.h"
#include "emitter.h"
#include "util.h"
#include "game.h"

Emitter::Emitter()
{
	tag = "Emitter";
	_lightMode = false;
	_target = NULL;
	on = false;
	autoKill = true;
	factory = NULL;
}

void Emitter::pos(float x, float y)
{
	pos(x, y, 0, 0);
}

void Emitter::pos(PointF p)
{
	pos(p.x, p.y, 0, 0);
}

void Emitter::pos(float x, float y, float width, float height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;

	_target = NULL;
}

void Emitter::pos(Visual* target)
{
	this->_target = target;
}

void Emitter::burst(Factory* factory, int quantity)
{
	start(factory, 0, quantity);
}

void Emitter::pour(Factory* factory, float interval)
{
	start(factory, interval, 0);
}

void Emitter::start(Factory* factory, float interval, int quantity)
{
	this->factory = factory;
	this->_lightMode = factory->lightMode();

	this->_interval = interval;
	this->_quantity = quantity;

	_count = 0;
	_time = Random::Float(0, interval);

	on = true;
}

void Emitter::update()
{
	if (on) {
		_time += Game::elapsed;
		while (_time > _interval) 
		{
			_time -= _interval;
			emit(_count++);
			if (_quantity > 0 && _count >= _quantity) 
			{
				on = false;
				break;
			}
		}
	}
	else if (autoKill && countLiving() == 0) 
	{
		kill();
	}

	Group::update();
}

void Emitter::emit(int index)
{
	if (_target == NULL) 
	{
		factory->emit(
			this,
			index,
			x + Random::Float(0, width),
			y + Random::Float(0, height));
	}
	else 
	{
		factory->emit(
			this,
			index,
			_target->x + Random::Float(0, _target->width),
			_target->y + Random::Float(0, _target->height));
	}
}

void Emitter::draw()
{
	if (_lightMode) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		Group::draw();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else {
		Group::draw();
	}
}