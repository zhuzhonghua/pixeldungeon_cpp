#include "button.h"
#include "game.h"

float Button::longClick = 1.0f;

Button::TouchArea1::TouchArea1(Button* btn) :TouchArea(0, 0, 0, 0)
{
	_btn = btn;
}

void Button::TouchArea1::onTouchDown(TouchScreen::Touch* touch)
{
	_btn->_pressed = true;
	_btn->_pressTime = 0;
	_btn->_processed = false;
	_btn->onTouchDown();
}

void Button::TouchArea1::onTouchUp(TouchScreen::Touch* touch)
{
	_btn->_pressed = false;
	_btn->onTouchUp();
}

void Button::TouchArea1::onClick(TouchScreen::Touch* touch)
{
	if (!_btn->_processed)
	{
		_btn->onClick();
	}
}

Button::Button()
{
	_pressed = false;
	_pressTime = 0;

	_processed = false;

	init();
}

void Button::layout()
{
	hotArea->x = _x;
	hotArea->y = _y;
	hotArea->width = _width;
	hotArea->height = _height;
}

void Button::createChildren()
{
	hotArea = new TouchArea1(this);
	add(hotArea);
}

void Button::update()
{
	Component::update();

	hotArea->active = visible;

	if (_pressed) 
	{
		if ((_pressTime += Game::elapsed) >= longClick) 
		{
			_pressed = false;
			if (onLongClick()) 
			{
				hotArea->reset();
				_pressed = true;
				onTouchUp();

				Game::vibrate(50);
			}
		}
	}
}