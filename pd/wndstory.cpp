#include "stdafx.h"

#include "wndstory.h"
#include "simpleresource.h"
#include "pixelscene.h"
#include "game.h"
#include "dungeon.h"
#include "bpt.h"

const float WndStory::bgR = 0.77f;
const float WndStory::bgG = 0.73f;
const float WndStory::bgB = 0.62f;

std::map<int, std::string> WndStory::CHAPTERS;

void WndStory::initChapters()
{
	if (CHAPTERS.size() <= 0)
	{
		CHAPTERS.insert(std::make_pair(ID_SEWERS, BPT::getText("lang.wndstory_sewers")));
		CHAPTERS.insert(std::make_pair(ID_PRISON, BPT::getText("lang.wndstory_prison")));
		CHAPTERS.insert(std::make_pair(ID_CAVES, BPT::getText("lang.wndstory_caves")));
		CHAPTERS.insert(std::make_pair(ID_METROPOLIS, BPT::getText("lang.wndstory_metropolis")));
		CHAPTERS.insert(std::make_pair(ID_HALLS, BPT::getText("lang.wndstory_halls")));
	}
}

namespace{
	class TouchAreaNew :public TouchArea{
	public:
		TouchAreaNew(WndStory* w, Visual* t) :wnd(w), TouchArea(t){}
	protected:
		WndStory* wnd;
		
		virtual void onClick(TouchScreen::Touch* touch) {
			wnd->hide();
			delete wnd;
		}
	};
}

WndStory::WndStory(const std::string& text)
:Window(0,0,Chrome::get(Chrome::SCROLL))
{
	initChapters();

	tf = PixelScene::createMultiline(text, 7);
	tf->maxWidth = WIDTH - MARGIN * 2;
	tf->measure();
	tf->ra = bgR;
	tf->ga = bgG;
	tf->ba = bgB;
	tf->rm = -bgR;
	tf->gm = -bgG;
	tf->bm = -bgB;
	tf->x = MARGIN;
	add(tf);

	add(new TouchAreaNew(this, chrome));

	resize((int)(tf->Width() + MARGIN * 2), (int)std::min((int)tf->Height(), 180));
}

void WndStory::update()
{
	Window::update();

	if (delay > 0 && (delay -= Game::elapsed) <= 0) 
	{
		shadow->visible = chrome->visible = tf->visible = true;
	}
}

void WndStory::showChapter(int id)
{
	initChapters();

	if (Dungeon::chapters.find(id) != Dungeon::chapters.end()) 
	{
		return;
	}

	std::map<int, std::string>::iterator itr = CHAPTERS.find(id);
	if (itr != CHAPTERS.end())
	{
		WndStory* wnd = new WndStory(itr->second);
		if ((wnd->delay = 0.6f) > 0) {
			wnd->shadow->visible = wnd->chrome->visible = wnd->tf->visible = false;
		}

		Game::scene()->add(wnd);

		Dungeon::chapters.insert(id);
	}
}