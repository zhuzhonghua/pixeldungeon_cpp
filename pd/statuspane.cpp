#include "statuspane.h"
#include "define.h"
#include "image.h"
#include "ninepatch.h"
#include "dungeon.h"
#include "hero.h"
#include "charsprite.h"
#include "camera.h"
#include "herosprite.h"
#include "bitmaskemitter.h"
#include "bloodparticle.h"
#include "compass.h"
#include "bitmaptext.h"
#include "pixelscene.h"
#include "dangerindicator.h"
#include "resumebutton.h"
#include "buffindicator.h"
#include "speck.h"

namespace{
	class TouchAreaNew :public TouchArea{
	public:
		TouchAreaNew(int x, int y, int w, int h) :TouchArea(x,y,w,h){}
	protected:
		virtual void onClick(TouchScreen::Touch* touch)
		{
			Image* sprite = Dungeon::hero->sprite;

			if (!sprite->isVisible()) 
			{
				Camera::mainCamera->focusOn(sprite);
			}
			//GameScene.show(new WndHero());
		}
	};
}

void StatusPane::layoutTags()
{
	float pos = 18;

	if (tagDanger) 
	{
		danger->setPos(_width - danger->width(), pos);
		pos = danger->bottom() + 1;
	}

	if (tagLoot) 
	{
		//loot.setPos(width - loot.width(), pos);
		//pos = loot.bottom() + 1;
	}

	if (tagResume) 
	{
		resume->setPos(_width - resume->width(), pos);
	}
}

void StatusPane::createChildren()
{
	shield = new NinePatch(Assets::STATUS, 80, 0, 30 + 18, 0);
	add(shield);

	add(new TouchAreaNew(0, 1, 30, 30));

	btnMenu = new MenuButton();
	add(btnMenu);

	avatar = HeroSprite::avatar(Dungeon::hero->heroClass, lastTier);
	add(avatar);

	blood = new BitmaskEmitter(avatar);
	blood->pour(BloodParticle::FACTORY, 0.3f);
	blood->autoKill = false;
	blood->on = false;
	add(blood);

	compass = new Compass(Dungeon::level->exit);
	add(compass);

	hp = new Image(Assets::HP_BAR);
	add(hp);

	exp = new Image(Assets::XP_BAR);
	add(exp);

	level = PixelScene::createText(9);//new BitmapText();
	level->hardlight(0xFFEBA4);
	add(level);

	depth = PixelScene::createText(GameMath::format("%d",Dungeon::depth), 9);// new BitmapText(Integer.toString(Dungeon.depth), PixelScene.font1x);
	depth->hardlight(0xCACFC2);
	depth->measure();
	add(depth);

	//Dungeon.hero.belongings.countIronKeys();
	keys = PixelScene::createText(9);
	keys->hardlight(0xCACFC2);
	add(keys);

	danger = new DangerIndicator();
	add(danger);

	//loot = new LootIndicator();
	//add(loot);

	resume = new ResumeButton();
	add(resume);

	buffs = new BuffIndicator(Dungeon::hero);
	add(buffs);
}

void StatusPane::layout()
{
	_height = 32;

	shield->size(_width, shield->height);

	avatar->x = PixelScene::align(camera(), shield->x + 15 - avatar->width / 2);
	avatar->y = PixelScene::align(camera(), shield->y + 16 - avatar->height / 2);

	compass->x = avatar->x + avatar->width / 2 - compass->origin.x;
	compass->y = avatar->y + avatar->height / 2 - compass->origin.y;

	hp->x = 30;
	hp->y = 3;

	depth->x = _width - 24 - depth->getWidth() - 18;
	depth->y = 6;

	keys->y = 6;

	layoutTags();

	buffs->setPos(32, 11);

	btnMenu->setPos(_width - btnMenu->width(), 1);
}

StatusPane::StatusPane()
{
	lastTier = 0;
	lastLvl = -1;
	lastKeys = -1;

	tagDanger = false;
	tagLoot = false;
	tagResume = false;

	init();
}

void StatusPane::update()
{
	Component::update();

	if (tagDanger != danger->visible /*|| tagLoot != loot.visible*/ || tagResume != resume->visible) {

		tagDanger = danger->visible;
		//tagLoot = loot.visible;
		tagResume = resume->visible;

		layoutTags();
	}

	float health = (float)Dungeon::hero->HP / Dungeon::hero->HT;

	if (health == 0) 
	{
		avatar->tint(0x000000, 0.6f);
		blood->on = false;
	}
	else if (health < 0.25f) {
		avatar->tint(0xcc0000, 0.4f);
		blood->on = true;
	}
	else {
		avatar->resetColor();
		blood->on = false;
	}

	hp->scale.x = health;
	exp->scale.x = (_width / exp->width) * Dungeon::hero->exp / Dungeon::hero->maxExp();

	if (Dungeon::hero->lvl != lastLvl) 
	{
		if (lastLvl != -1) 
		{
			Emitter* emitter = (Emitter*)recycle("Emitter");
			if (emitter == NULL)
			{
				emitter = new Emitter();
				add(emitter);
			}
			emitter->revive();
			emitter->pos(27, 27);
			emitter->burst(Speck::factory(Speck::STAR), 12);
		}

		lastLvl = Dungeon::hero->lvl;
		level->text(GameMath::format("%d", lastLvl));
		level->measure();
		level->x = PixelScene::align(27.5f - level->getWidth() / 2);
		level->y = PixelScene::align(28.0f - level->baseLine() / 2);
	}

	//int k = IronKey.curDepthQuantity;
	//if (k != lastKeys) {
	//	lastKeys = k;
	//	keys.text(Integer.toString(lastKeys));
	//	keys.measure();
	//	keys.x = width - 8 - keys.width() - 18;
	//}

	int tier = Dungeon::hero->tier();
	if (tier != lastTier) 
	{
		lastTier = tier;
		Image* img = HeroSprite::avatar(Dungeon::hero->heroClass, tier);
		avatar->copy(img);
		delete img;
	}
}

void StatusPane::MenuButton::createChildren()
{
	image = new Image(Assets::STATUS, 114, 3, 12, 11);
	add(image);
}

void StatusPane::MenuButton::layout()
{
	Button::layout();

	image->x = _x + 2;
	image->y = _y + 2;
}

void StatusPane::MenuButton::onTouchDown()
{
	image->brightness(1.5f);
	//Sample.INSTANCE.play(Assets.SND_CLICK);
}

void StatusPane::MenuButton::onTouchUp()
{
	image->resetColor();
}

void StatusPane::MenuButton::onClick()
{
	//GameScene::show(new WndGame());
}

StatusPane::MenuButton::MenuButton()
{
	init();

	_width = image->width + 4;
	_height = image->height + 4;
}
