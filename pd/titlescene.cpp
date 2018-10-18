#include "stdafx.h"

#include "titlescene.h"
#include "image.h"
#include "bannersprites.h"
#include "pixeldungeon.h"
#include "archs.h"
#include "fireball.h"
#include "pixelparticle.h"
#include "bpt.h"
#include "startscene.h"

namespace{
	class DashboardItemBadge :public DashboardItem{
	public:
		DashboardItemBadge(const std::string& text, int index) :DashboardItem(text, index){}
	protected:
		virtual void onClick() 
		{
			//PixelDungeon.switchNoFade(BadgesScene.class);
			printf("test1");
		}
	};
	class DashboardItemAbout :public DashboardItem{
	public:
		DashboardItemAbout(const std::string& text, int index) :DashboardItem(text, index){}
	protected:
		virtual void onClick()
		{
			//PixelDungeon.switchNoFade(BadgesScene.class);
			printf("test2");
		}
	};
	class DashboardItemPlay :public DashboardItem{
	public:
		DashboardItemPlay(const std::string& text, int index) :DashboardItem(text, index){}
	protected:
		virtual void onClick()
		{
			PixelDungeon::switchNoFade(new StartScene());
			//printf("test3");
		}
	};
	class DashboardItemHighScores :public DashboardItem{
	public:
		DashboardItemHighScores(const std::string& text, int index) :DashboardItem(text, index){}
	protected:
		virtual void onClick()
		{
			//PixelDungeon.switchNoFade(BadgesScene.class);
			printf("test4");
		}
	};
}
void TitleScene::init()
{
	PixelScene::init();

	uiCamera->visible = false;
	//
	int w = Camera::mainCamera->width;
	int h = Camera::mainCamera->height;
	//
	Archs* archs = new Archs();
	archs->setSize(w, h);
	add(archs);
	
	Image* title = BannerSprites::get(BannerSprites::Type::PIXEL_DUNGEON);
	add(title);
	
	float height = title->height +
		(PixelDungeon::landscape() ? DashboardItem_SIZE : DashboardItem_SIZE * 2);
	
	title->x = (w - title->getWidth()) / 2;
	title->y = (h - height) / 2;
	
	placeTorch(title->x + 18, title->y + 20);
	placeTorch(title->x + title->width - 18, title->y + 20);
	
	Image* signs = new SignImage(BannerSprites::get(BannerSprites::Type::PIXEL_DUNGEON_SIGNS));
	//Image* signs = BannerSprites::get(BannerSprites::Type::PIXEL_DUNGEON_SIGNS);
	signs->x = title->x;
	signs->y = title->y;
	add(signs);

	DashboardItem* btnBadges = new DashboardItemBadge(TXT_BADGES, 3);
	add(btnBadges);
	DashboardItem* btnAbout = new DashboardItemAbout(TXT_ABOUT, 1);
	add(btnAbout);
	DashboardItem* btnPlay = new DashboardItemPlay(TXT_PLAY, 0);
	add(btnPlay);
	DashboardItem* btnHighscores = new DashboardItemHighScores(TXT_HIGHSCORES, 2);
	add(btnHighscores);

	if (PixelDungeon::landscape()) 
	{
		float y = (h + height) / 2 - DashboardItem_SIZE;
		btnHighscores->setPos(w / 2 - btnHighscores->width(), y);
		btnBadges->setPos(w / 2, y);
		btnPlay->setPos(btnHighscores->left() - btnPlay->width(), y);
		btnAbout->setPos(btnBadges->right(), y);
	}
	else 
	{
		btnBadges->setPos(w / 2 - btnBadges->width(), (h + height) / 2 - DashboardItem_SIZE);
		btnAbout->setPos(w / 2, (h + height) / 2 - DashboardItem_SIZE);
		btnPlay->setPos(w / 2 - btnPlay->width(), btnAbout->top() - DashboardItem_SIZE);
		btnHighscores->setPos(w / 2, btnPlay->top());
	}

	fadeIn();
}

void TitleScene::placeTorch(float x, float y)
{
	Fireball* fb = new Fireball();
	fb->setPos(x, y);
	add(fb);
}
SignImage::SignImage(Image* img) :Image(*img)
{
	time = 0;
	delete img;
}

void SignImage::update()
{
	Image::update();
	am = (float)std::sin(-(time += Game::elapsed));
}

void SignImage::draw()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	Image::draw();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

DashboardItem::DashboardItem(const std::string& text, int index)
{
	init();

	image->frame(image->tex->uvRect(index * IMAGE_SIZE, 0, (index + 1) * IMAGE_SIZE, IMAGE_SIZE));

	this->label->text(BPT::getText(text));
	this->label->measure();

	setSize(DashboardItem_SIZE, DashboardItem_SIZE);
}

void DashboardItem::createChildren()
{
	Button::createChildren();

	image = new Image(Assets::DASHBOARD);
	add(image);

	label = TitleScene::createText(9);
	add(label);
}

void DashboardItem::layout()
{
	Button::layout();

	image->x = TitleScene::align(_x + (_width - image->getWidth()) / 2);
	image->y = TitleScene::align(_y);

	label->x = TitleScene::align(_x + (_width - label->getWidth()) / 2);
	label->y = TitleScene::align(image->y + image->getHeight() + 2);
}

void DashboardItem::onTouchDown()
{
	image->brightness(1.5f);
	//Sample.INSTANCE.play(Assets.SND_CLICK, 1, 1, 0.8f);
}

void DashboardItem::onTouchUp()
{
	image->resetColor();
}