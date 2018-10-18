#include "redbutton.h"
#include "simpleresource.h"
#include "pixelscene.h"

RedButton::RedButton(const std::string& label)
{
	_icon = NULL;

	init();

	_text->text(label);
	_text->measure();
}

void RedButton::createChildren() 
{
	//Button::createChildren();

	_bg = Chrome::get(Chrome::Type::BUTTON);
	add(_bg);

	//text = PixelScene.createText(9);
	_text = PixelScene::createText(9);
	add(_text);
}

void RedButton::layout() 
{
	Button::layout();

	_bg->x = _x;
	_bg->y = _y;
	_bg->size(_width, _height);

	_text->x = _x + (int)(_width - _text->getWidth()) / 2;
	_text->y = _y + (int)(_height - _text->baseLine()) / 2;

	if (_icon != NULL) 
	{
		_icon->x = _x + _text->x - _icon->getWidth() - 2;
		_icon->y = _y + (_height - _icon->getHeight()) / 2;
	}
};

void RedButton::onTouchDown() 
{
	_bg->brightness(1.2f);
	//Sample.INSTANCE.play(Assets.SND_CLICK);
};

void RedButton::onTouchUp() {
	//bg.resetColor();
};

void RedButton::enable(bool value) 
{
	active = value;
	_text->alpha(value ? 1.0f : 0.3f);
}

void RedButton::text(const std::string& label)
{
	_text->text(label);
	_text->measure();
	layout();
}

void RedButton::textColor(int value) 
{
	_text->hardlight(value);
}

void RedButton::icon(Image* icon) 
{
	if (this->_icon != NULL) 
	{
		remove(this->_icon);
	}
	this->_icon = icon;
	if (this->_icon != NULL) 
	{
		add(this->_icon);
		layout();
	}
}

float RedButton::reqWidth() 
{
	return _text->getWidth() + 4;
}

float RedButton::reqHeight() 
{
	return _text->baseLine() + 4;
}