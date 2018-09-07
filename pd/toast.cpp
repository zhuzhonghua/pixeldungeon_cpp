#include "toast.h"
#include "simpleresource.h"
#include "icon.h"
#include "pixelscene.h"
#include "bitmaptext.h"

void Toast::createChildren()
{
	bg = Chrome::get(Chrome::Type::TOAST_TR);
	add(bg);

	Image* img = Icons::get(Icons::CLOSE);
	close = new SimpleButtonNew(this, img);
	add(close);

	text = PixelScene::createText(8);
	add(text);
}

void Toast::layout()
{
	Component::layout();

	bg->x = _x;
	bg->y = _y;
	bg->size(_width, _height);

	close->setPos(
		bg->x + bg->Width() - bg->marginHor() / 2 - MARGIN_HOR - close->width(),
		_y + (_height - close->height()) / 2);

	text->x = close->left() - MARGIN_HOR - text->Width();
	text->y = _y + (_height - text->Height()) / 2;
	PixelScene::align(text);
}

Toast::Toast(const std::string& t)
{
	init();

	Text(t);

	_width = text->Width() + close->width() + bg->marginHor() + MARGIN_HOR * 3;
	_height = std::max(text->Height(), close->height()) + bg->marginVer() + MARGIN_VER * 2;
}

void Toast::Text(const std::string& txt)
{
	text->text(txt);
	text->measure();
}
