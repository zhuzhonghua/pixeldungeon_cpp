#include "toucharea.h"

TouchArea::TouchArea(Visual* target) :Visual(0, 0, 0, 0)
{
	this->target = target;
	_init();

	TouchScreen::event.add(this);
}

TouchArea::TouchArea(float x, float y, float width, float height) : Visual(x, y, width, height)
{
	this->target = this;
	visible = false;

	_init();

	TouchScreen::event.add(this);
}

void TouchArea::_init()
{
	_touch = NULL;
}

TouchArea::~TouchArea()
{
	destroy();
}

void TouchArea::destroy()
{
	TouchScreen::event.remove(this);
	Visual::destroy();
}

void TouchArea::onSignal(TouchScreen::Touch* touch)
{
	if (!isActive()) 
	{
		return;
	}

	bool hit = touch != NULL && target->overlapsScreenPoint((int)touch->start.x, (int)touch->start.y);

	if (hit) 
	{
		TouchScreen::event.cancel();

		if (touch->down) 
		{

			if (this->_touch == NULL) 
			{
				this->_touch = touch;
			}
			onTouchDown(touch);

		}
		else 
		{
			onTouchUp(touch);

			if (this->_touch == touch) 
			{
				this->_touch = NULL;
				onClick(touch);
			}
		}
	}
	else 
	{
		if (touch == NULL && this->_touch != NULL) 
		{
			onDrag(this->_touch);
		}

		else if (this->_touch != NULL && touch != NULL && !touch->down)
		{
			onTouchUp(touch);
			this->_touch = NULL;
		}
	}
}