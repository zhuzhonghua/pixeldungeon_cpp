#include "simplebutton.h"
#include "toucharea.h"
#include "image.h"

SimpleButton::SimpleButton(Image* image)
{
	init();

	this->image->copy(image);
	_width = image->width;
	_height = image->height;
}

void SimpleButton::createChildren()
{
	image = new Image();
	add(image);

	add(new TouchAreaNew(this, image));
}

void SimpleButton::layout()
{
	image->x = _x;
	image->y = _y;
}

SimpleButton::TouchAreaNew::TouchAreaNew(SimpleButton* sbp, Image* img)
:TouchArea(img),
sb(sbp)
{

}

void SimpleButton::TouchAreaNew::onTouchDown(TouchScreen::Touch* touch)
{
	sb->image->brightness(1.2f);
}

void SimpleButton::TouchAreaNew::onTouchUp(TouchScreen::Touch* touch)
{
	sb->image->brightness(1.0f);
}

void SimpleButton::TouchAreaNew::onClick(TouchScreen::Touch* touch)
{
	sb->onClick();
}
