#include "wndchallenges.h"
#include "pixelscene.h"
#include "define.h"
#include "pixeldungeon.h"
#include "bpt.h"

const std::string WndChallenges::TITLE = "lang.wndchallenge_title";

WndChallenges::WndChallenges(int checked, bool editable)
{
	this->editable = editable;

	BitmapText* title = PixelScene::createText(BPT::getText(TITLE), 9);
	title->hardlight(TITLE_COLOR);
	title->measure();
	title->x = PixelScene::align(cameraf, (WIDTH - title->Width()) / 2);
	title->y = PixelScene::align(cameraf, (TTL_HEIGHT - title->Height()) / 2);
	add(title);

	float pos = TTL_HEIGHT;
	int cnl = Challenges::NAME_LEN;

	for (int i = 0; i < cnl; i++) 
	{
		CheckBoxRed* cb = new CheckBoxRed(BPT::getText(Challenges::NAMES[i]));
		cb->Checked((checked & Challenges::MASKS[i]) != 0);
		cb->active = editable;

		if (i > 0) 
		{
			pos += GAP;
		}
		cb->setRect(0, pos, WIDTH, BTN_HEIGHT);
		pos = cb->bottom();

		add(cb);
		boxes.push_back(cb);
	}

	resize(WIDTH, (int)pos);
}

void WndChallenges::onBackPressed()
{
	if (editable) 
	{
		int value = 0;
		for (int i = 0; i < boxes.size(); i++) 
		{
			if (boxes[i]->Checked()) 
			{
				value |= Challenges::MASKS[i];
			}
		}
		PixelDungeon::challenges(value);
	}

	Window::onBackPressed();
}