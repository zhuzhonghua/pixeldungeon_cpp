#include "amuletscene.h"
#include "bpt.h"
#include "redbutton.h"
#include "dungeon.h"
#include "game.h"
#include "simpleresource.h"
#include "bitmaptextmultiline.h"
#include "flare.h"
#include "interlevelscene.h"
#include "speck.h"
#include "hero.h"

const String AmuletScene::TXT_EXIT = BPT::getText("lang.AmuletScene_Exit");// "Let's call it a day";

const String AmuletScene::TXT_STAY = BPT::getText("lang.AmuletScene_Stay");// "I'm not done yet";

const float AmuletScene::SMALL_GAP = 2;

const float AmuletScene::LARGE_GAP = 8;

const String AmuletScene::TXT = BPT::getText("lang.AmuletScene_Txt");
//"You finally hold it in your hands, the Amulet of Yendor. Using its power " +
//"you can take over the world or bring peace and prosperity to people or whatever. " +
//"Anyway, your life will change forever and this game will end here. " +
//"Or you can stay a mere mortal a little longer.";

boolean AmuletScene::noText = false;

namespace{
	class RedButtonExit :public RedButton{
	public:
		RedButtonExit(const std::string& txt) :RedButton(txt){}
	protected:
		virtual void onClick(){
			Dungeon::win(ResultDescriptions::WIN);
			Dungeon::deleteGame(Dungeon::hero->heroClass, true);
			//Game.switchScene(noText ? TitleScene.class : RankingsScene.class);
		}
	};
	class RedButtonStay :public RedButton{
	public:
		AmuletScene* ins;
		RedButtonStay(AmuletScene* as, const std::string& txt) :RedButton(txt),ins(as){}

	protected:
		virtual void onClick() {
			ins->onBackPressed();
		}
	};
}
void AmuletScene::init()
{
	PixelScene::init();

	BitmapTextMultiline* text = null;
	if (!noText) {
		text = createMultiline(TXT, 8);
		text->maxWidth = WIDTH;
		text->measure();
		add(text);
	}

	amulet = new Image(Assets::AMULET);
	add(amulet);

	RedButton* btnExit = new RedButtonExit(TXT_EXIT);
	btnExit->setSize(WIDTH, BTN_HEIGHT);
	add(btnExit);

	RedButton* btnStay = new RedButtonStay(this, TXT_STAY);
	btnStay->setSize(WIDTH, BTN_HEIGHT);
	add(btnStay);

	float height;
	if (noText) {
		height = amulet->height + LARGE_GAP + btnExit->height() + SMALL_GAP + btnStay->height();

		amulet->x = align((Camera::mainCamera->width - amulet->width) / 2);
		amulet->y = align((Camera::mainCamera->height - height) / 2);

		btnExit->setPos((Camera::mainCamera->width - btnExit->width()) / 2, amulet->y + amulet->height + LARGE_GAP);
		btnStay->setPos(btnExit->left(), btnExit->bottom() + SMALL_GAP);

	}
	else {
		height = amulet->height + LARGE_GAP + text->Height() + LARGE_GAP + btnExit->height() + SMALL_GAP + btnStay->height();

		amulet->x = align((Camera::mainCamera->width - amulet->width) / 2);
		amulet->y = align((Camera::mainCamera->height - height) / 2);

		text->x = align((Camera::mainCamera->width - text->Width()) / 2);
		text->y = amulet->y + amulet->height + LARGE_GAP;

		btnExit->setPos((Camera::mainCamera->width - btnExit->width()) / 2, text->y + text->Height() + LARGE_GAP);
		btnStay->setPos(btnExit->left(), btnExit->bottom() + SMALL_GAP);
	}

	Flare* flare = new Flare(8, 48);
	flare->color(0xFFDDBB, true);
	flare->show(amulet, 0);
	flare->angularSpeed = +30;

	fadeIn();
}

void AmuletScene::onBackPressed()
{
	InterlevelScene::mode = InterlevelScene::Mode::CONTINUE;
	Game::switchScene(new InterlevelScene);
}

AmuletScene::AmuletScene()
{
	timer = 0;
}

void AmuletScene::update()
{
	PixelScene::update();

	if ((timer -= Game::elapsed) < 0) {
		timer = Random::Float(0.5f, 5.0f);

		RECYCLE2(star, this, Speck);
		star->reset(0, amulet->x + 10.5f, amulet->y + 5.5f, Speck::DISCOVER);		
	}
}
