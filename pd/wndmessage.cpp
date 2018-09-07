#include "wndmessage.h"
#include "bitmaptextmultiline.h"
#include "pixelscene.h"
#include "pixeldungeon.h"

WndMessage::WndMessage(const std::string& text)
{
	BitmapTextMultiline* info = PixelScene::createMultiline(text, 6);
	info->maxWidth = (PixelDungeon::landscape() ? WIDTH_L : WIDTH_P) - MARGIN * 2;
	info->measure();
	info->x = info->y = MARGIN;
	add(info);

	resize(
		(int)info->Width() + MARGIN * 2,
		(int)info->Height() + MARGIN * 2);
}