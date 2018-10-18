#include "highlightedtext.h"
#include "pixelscene.h"
#include "highlighter.h"

HighlightedText::HighlightedText(float size)
{
	normal = NULL;
	highlighted = NULL;

	nColor = 0xFFFFFF;
	hColor = 0xFFFF44;

	init();

	normal = PixelScene::createMultiline(size);
	add(normal);

	highlighted = PixelScene::createMultiline(size);
	add(highlighted);

	setColor(0xFFFFFF, 0xFFFF44);
}

void HighlightedText::layout()
{
	normal->x = highlighted->x = _x;
	normal->y = highlighted->y = _y;
}

void HighlightedText::text(const std::string& value, int maxWidth)
{
	Highlighter hl(value);

	normal->text(hl.text);
	normal->maxWidth = maxWidth;
	normal->measure();

	if (hl.isHighlighted()) 
	{
		hl.inverted(normal->mask);

		highlighted->text(hl.text);
		highlighted->maxWidth = maxWidth;
		highlighted->measure();

		highlighted->mask = hl.mask;
		highlighted->visible = true;
	}
	else 
	{
		highlighted->visible = false;
	}

	_width = normal->getWidth();
	_height = normal->getHeight();
}

void HighlightedText::setColor(int n, int h)
{
	normal->hardlight(n);
	highlighted->hardlight(h);
}