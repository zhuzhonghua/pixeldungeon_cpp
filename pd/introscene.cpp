#include "introscene.h"

#include "wndstory.h"
#include "game.h"
#include "interlevelscene.h"
#include "bpt.h"

const std::string IntroScene::TEXT = "lang.intro";

namespace{
	class WndStoryNew :public WndStory{
	public:
		WndStoryNew(const std::string& txt) :WndStory(txt){}

		virtual void hide()
		{
			WndStory::hide();
			Game::switchScene(new InterlevelScene());
			delete this;
		}
	};
}
void IntroScene::init()
{
	PixelScene::init();
	add(new WndStoryNew(BPT::getText(TEXT)));
	fadeIn();
}