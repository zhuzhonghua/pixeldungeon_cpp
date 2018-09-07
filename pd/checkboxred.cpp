#include "checkboxred.h"
#include "icon.h"
#include "pixelscene.h"

CheckBoxRed::CheckBoxRed(const std::string& label)
:checked(false)
,RedButton(label)
{
	icon(Icons::get(Icons::UNCHECKED));
}

void CheckBoxRed::layout()
{
	RedButton::layout();

	float margin = (_height - _text->baseLine()) / 2;

	_text->x = PixelScene::align(PixelScene::uiCamera, _x + margin);
	_text->y = PixelScene::align(PixelScene::uiCamera, _y + margin);

	margin = (_height - _icon->height) / 2;

	_icon->x = PixelScene::align(PixelScene::uiCamera, _x + _width - margin - _icon->width);
	_icon->y = PixelScene::align(PixelScene::uiCamera, _y + margin);
}

void CheckBoxRed::Checked(bool value)
{
	if (checked != value) 
	{
		checked = value;
		Image* ic = Icons::get(checked ? Icons::CHECKED : Icons::UNCHECKED);
		_icon->copy(*ic);
		delete ic;
	}
}

void CheckBoxRed::onClick()
{
	RedButton::onClick();
	Checked(!checked);
}