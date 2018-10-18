#include "startscene.h"
#include "pixeldungeon.h"
#include "archs.h"
#include "bannersprites.h"
#include "bitmaskemitter.h"
#include "speck.h"
#include "icon.h"
#include "exitbutton.h"
#include "bpt.h"
#include "titlescene.h"
#include "badge.h"
#include "gamesinprogress.h"
#include "wndoptions.h"
#include "wndclass.h"
#include "wndmessage.h"
#include "wndchallenges.h"
#include "dungeon.h"
#include "interlevelscene.h"
#include "introscene.h"

const char* StartScene::TXT_LOAD = "lang.loadgame";
const char* StartScene::TXT_NEW = "lang.newgame";

const char* StartScene::TXT_ERASE = "lang.erase_current_game";
const char* StartScene::TXT_DPTH_LVL = "lang.depth_lvl";

const char* StartScene::TXT_REALLY = "lang.really";
const char* StartScene::TXT_WARNING = "lang.start_scene_warning";
const char* StartScene::TXT_YES = "lang.start_scene_yes";
const char* StartScene::TXT_NO = "lang.start_scene_no";

const char* StartScene::TXT_UNLOCK = "lang.unlock";

const char* StartScene::TXT_WIN_THE_GAME = "lang.unlock_challenge";

namespace{
	class NewWndOptions :public WndOptions{
	public:
		StartScene* _scene;
		NewWndOptions(StartScene* sce, const std::string& title, const std::string& message, const std::vector<std::string>& options)
			:WndOptions(title,message,options)
		{
			this->_scene = sce;
		}
	protected:
		virtual void onSelect(int index) 
		{
			if (index == 0) 
			{
				_scene->startNewGame();
			}
		}
	};
	class NewGameButton :public StartScene::GameButton{
	public:
		NewGameButton(const std::string& txt, StartScene* scene) :StartScene::GameButton(txt)
		{
			_scene = scene;
		}
	protected:
		StartScene* _scene;
		virtual void onClick()
		{
			GamesInProgress::Info info;

			if (GamesInProgress::check(_scene->curClass, info))
			//if (true)
			{
				std::vector<std::string> options;
				options.push_back(BPT::getText(StartScene::TXT_YES));
				options.push_back(BPT::getText(StartScene::TXT_NO));

				_scene->add(new NewWndOptions(_scene, BPT::getText(StartScene::TXT_REALLY), BPT::getText(StartScene::TXT_WARNING), options));
			}
			else
			{
				_scene->startNewGame();
			}
		}
	};

	class LoadGameButton :public StartScene::GameButton{
	public:
		LoadGameButton(const std::string& txt, StartScene* scene) :StartScene::GameButton(txt)
		{
			_scene = scene;
		}
	protected:
		StartScene* _scene;
		virtual void onClick()
		{
			printf("test2");
		}
	};
}

StartScene::GameButton::GameButton(const std::string& primary) :RedButton(primary)
{
	init();

	_secondary->text("");
}

void StartScene::GameButton::createChildren()
{
	//RedButton::createChildren();
	_secondary = StartScene::createText(6);// new BitmapText();
	add(_secondary);
}

void StartScene::GameButton::layout()
{
	RedButton::layout();

	if (_secondary->text().length() > 0) 
	{
		_text->y = align(_y + (_height - _text->getHeight() - _secondary->baseLine()) / 2);

		_secondary->x = align(_x + (_width - _secondary->getWidth()) / 2);
		_secondary->y = align(_text->y + _text->getHeight());
	}
	else 
	{
		_text->y = align(_y + (_height - _text->baseLine()) / 2);
	}
}

void StartScene::GameButton::secondary(const std::string& text, bool highlighted)
{
	_secondary->text(text);
	_secondary->measure();

	_secondary->hardlight(highlighted ? SECONDARY_COLOR_H : SECONDARY_COLOR_N);
}

const float StartScene::ClassShield::MIN_BRIGHTNESS = 0.6f;
StartScene* StartScene::ClassShield::startScene = NULL;

StartScene::ClassShield::ClassShield(HeroClass clp) :cl(clp)
{
	init();
	
	avatar->frame(cl.ordinal() * WIDTH, 0, WIDTH, HEIGHT);
	GameMath::PointFSet(&avatar->scale, SCALE);
	//avatar->scale.set(SCALE);

	normal = BASIC_NORMAL;
	highlighted = BASIC_HIGHLIGHTED;

	if (Badges::isUnlocked(cl.masteryBadge())) 
	{
		normal = MASTERY_NORMAL;
		highlighted = MASTERY_HIGHLIGHTED;
	}
	else 
	{
		normal = BASIC_NORMAL;
		highlighted = BASIC_HIGHLIGHTED;
	}

	name->text(cl.name());
	name->measure();
	name->hardlight(normal);

	brightness = MIN_BRIGHTNESS;
	updateBrightness();
}

void StartScene::ClassShield::createChildren()
{
	Button::createChildren();

	avatar = new Image(Assets::AVATARS);
	add(avatar);

	name = StartScene::createText(9);// new BitmapText();
	add(name);

	emitter = new BitmaskEmitter(avatar);
	add(emitter);
}

void StartScene::ClassShield::layout()
{
	Button::layout();

	avatar->x = align(_x + (_width - avatar->getWidth()) / 2);
	avatar->y = align(_y + (_height - avatar->getHeight() - name->getHeight()) / 2);

	name->x = align(_x + (_width - name->getWidth()) / 2);
	name->y = avatar->y + avatar->getHeight() + SCALE;
}

void StartScene::ClassShield::onTouchDown()
{
	emitter->revive();
	emitter->start(Speck::factory(Speck::LIGHT), 0.05f, 7);

	//Sample.INSTANCE.play(Assets.SND_CLICK, 1, 1, 1.2f);
	startScene->updateClass(cl);
}

void StartScene::ClassShield::update()
{
	Button::update();

	if (brightness < 1.0f && brightness > MIN_BRIGHTNESS) 
	{
		if ((brightness -= Game::elapsed) <= MIN_BRIGHTNESS) 
		{
			brightness = MIN_BRIGHTNESS;
		}
		updateBrightness();
	}
}

void StartScene::ClassShield::highlight(bool value)
{
	if (value) 
	{
		brightness = 1.0f;
		name->hardlight(highlighted);
	}
	else 
	{
		brightness = 0.999f;
		name->hardlight(normal);
	}

	updateBrightness();
}

void StartScene::ClassShield::updateBrightness()
{
	avatar->gm = avatar->bm = avatar->rm = avatar->am = brightness;
}

StartScene::ChallengeButton::ChallengeButton(StartScene* sce)
{
	scene = sce;

	init();

	_width = image->width;
	_height = image->height;

	image->am = Badges::isUnlocked(Badges::VICTORY) ? 1.0f : 0.5f;
}

void StartScene::ChallengeButton::createChildren()
{
	//Button::createChildren();

	image = Icons::get(PixelDungeon::challenges() > 0 ? Icons::CHALLENGE_ON : Icons::CHALLENGE_OFF);
	add(image);
}

void StartScene::ChallengeButton::layout()
{
	Button::layout();

	image->x = align(_x);
	image->y = align(_y);
}

namespace{
	class WndChallengesNew:public WndChallenges{
	public:
		StartScene::ChallengeButton* cb;
		WndChallengesNew(StartScene::ChallengeButton* cb1, int checked, bool editable)
			:cb(cb1)
			,WndChallenges(checked, editable)
		{

		}
		virtual void onBackPressed()
		{
			Image* img = Icons::get(PixelDungeon::challenges() > 0 ? Icons::CHALLENGE_ON : Icons::CHALLENGE_OFF);
			cb->image->copy(*img);
			delete img;

			WndChallenges::onBackPressed();
		}
	};
}
void StartScene::ChallengeButton::onClick()
{
	if (Badges::isUnlocked(Badges::VICTORY))
	{
		scene->add(new WndChallengesNew(this, PixelDungeon::challenges(), true));
	}
	else 
	{
		scene->add(new WndMessage(BPT::getText(TXT_WIN_THE_GAME)));
	}
}

void StartScene::ChallengeButton::onTouchDown()
{
	//Sample.INSTANCE.play( Assets.SND_CLICK );
}

HeroClass StartScene::curClass("");
std::map<HeroClass, StartScene::ClassShield*> StartScene::shields;

namespace{
	class CallbackNew :public Callback{
	public:
		StartScene* sce;
		CallbackNew(StartScene* s) :sce(s){}

		virtual void call() 
		{
			if (Game::scene() == sce)
			{
				PixelDungeon::switchNoFade(new StartScene());
			}
		}
	};
}
void StartScene::init()
{
	PixelScene::init();

	Badges::loadGlobal();

	uiCamera->visible = false;

	int w = Camera::mainCamera->width;
	int h = Camera::mainCamera->height;

	float width, height;
	if (PixelDungeon::landscape()) 
	{
		width = WIDTH_L;
		height = HEIGHT_L;
	}
	else 
	{
		width = WIDTH_P;
		height = HEIGHT_P;
	}

	float left = (w - width) / 2;
	float top = (h - height) / 2;
	float bottom = h - top;

	Archs* archs = new Archs();
	archs->setSize(w, h);
	add(archs);
	
	Image* title = BannerSprites::get(BannerSprites::Type::SELECT_YOUR_HERO);
	title->x = align((w - title->getWidth()) / 2);
	title->y = align(top);
	add(title);
	
	buttonX = left;
	buttonY = bottom - BUTTON_HEIGHT;
	
	btnNewGame = new NewGameButton(BPT::getText(TXT_NEW), this);
	add(btnNewGame);
	btnLoad = new LoadGameButton(BPT::getText(TXT_LOAD), this);
	add(btnLoad);
	
	float centralHeight = buttonY - title->y - title->getHeight();

	ClassShield::startScene = this;

	const HeroClass classes[] = {
		HeroClass(HeroClass::WARRIOR), 
		HeroClass(HeroClass::MAGE), 
		HeroClass(HeroClass::ROGUE), 
		HeroClass(HeroClass::HUNTRESS)
	};

	const int CLASSES_LEN = sizeof(classes) / sizeof(HeroClass);
	//const int CLASSES_LEN = 1;
	for (int i = 0; i < CLASSES_LEN; i++)
	{
		const HeroClass& cl = classes[i];
		ClassShield* shield = new ClassShield(cl);
		shields.insert(std::make_pair(cl, shield));
		add(shield);
	}
	
	if (PixelDungeon::landscape()) 
	{
		float shieldW = width / 4;
		float shieldH = std::min(centralHeight, shieldW);
		top = title->y + title->height + (centralHeight - shieldH) / 2;
		for (int i = 0; i < CLASSES_LEN; i++) 
		{
			ClassShield* shield = shields.find(classes[i])->second;
			shield->setRect(left + i * shieldW, top, shieldW, shieldH);
		}
	
		ChallengeButton* challenge = new ChallengeButton(this);
		challenge->setPos(
			w / 2 - challenge->width() / 2,
			top + shieldH - challenge->height() / 2);
		add(challenge);
	
	}
	else 
	{
		float shieldW = width / 2;
		float shieldH = std::min(centralHeight / 2, shieldW * 1.2f);
		top = title->y + title->getHeight() + centralHeight / 2 - shieldH;
		for (int i = 0; i < CLASSES_LEN; i++) 
		{
			ClassShield* shield = shields.find(classes[i])->second;
			shield->setRect(
				left + (i % 2) * shieldW,
				top + (i / 2) * shieldH,
				shieldW, shieldH);
		}
	
		ChallengeButton* challenge = new ChallengeButton(this);
		challenge->setPos(
			w / 2 - challenge->width() / 2,
			top + shieldH - challenge->height() / 2);
		add(challenge);
	}

	unlock = new Group();
	add(unlock);

	if (!(huntressUnlocked = Badges::isUnlocked(Badges::BOSS_SLAIN_3))) 	
	{	
		BitmapTextMultiline* txt = PixelScene::createMultiline(BPT::getText(TXT_UNLOCK), 9);
		txt->maxWidth = (int)width;
		txt->measure();
		
		float pos = (bottom - BUTTON_HEIGHT) + (BUTTON_HEIGHT - txt->getHeight()) / 2;
		BitmapTextMultiline::LineSplitter lineSpt(txt);

		std::vector<BitmapText*> lines;
		lineSpt.split(lines);

		for (int i = 0; i < lines.size();i++)
		{
			BitmapText* line = lines[i];

			line->measure();
			line->hardlight(0xFFFF00);
			line->x = PixelScene::align(w / 2 - line->getWidth() / 2);
			line->y = PixelScene::align(pos);
			unlock->add(line);
		
			pos += line->getHeight();
		}
		delete txt;
	}

	ExitButton* btnExit = new ExitButton();
	btnExit->setPos(Camera::mainCamera->width - btnExit->width(), 0);
	add(btnExit);

	curClass.setNull();

	updateClass(classes[0]);

	fadeIn();

	Badges::loadingListener = new CallbackNew(this);
}

StartScene::StartScene()
{

}

StartScene::~StartScene()
{
	destroy();
}

void StartScene::destroy()
{
	Badges::saveGlobal();
	if (Badges::loadingListener != NULL)
	{
		delete Badges::loadingListener;
		Badges::loadingListener = NULL;
	}

	PixelScene::destroy();

	shields.clear();
}

void StartScene::updateClass(HeroClass cl)
{
	if (curClass == cl) {
		add(new WndClass(cl));
		return;
	}

	if (curClass.isNull() == false) 
	{
		shields.find(curClass)->second->highlight(false);
	}
	curClass = cl;
	shields.find(curClass)->second->highlight(true);

	if (cl.type() != HeroClass::E_HUNTRESS || huntressUnlocked) 
	{
		unlock->visible = false;
	
		GamesInProgress::Info info(1,1,0);
		bool flag = GamesInProgress::check(curClass, info);

		if (flag) 
		{		
			btnLoad->visible = true;
			std::string tmp = GameMath::format(BPT::getText(TXT_DPTH_LVL).c_str(), info.depth, info.level);
			btnLoad->secondary(tmp, info.challenges);
		
			btnNewGame->visible = true;
			btnNewGame->secondary(BPT::getText(TXT_ERASE), false);
		
			float w = (Camera::mainCamera->width - GAP) / 2 - buttonX;
		
			btnLoad->setRect(buttonX, buttonY, w, BUTTON_HEIGHT);
			btnNewGame->setRect(btnLoad->right() + GAP, buttonY, w, BUTTON_HEIGHT);		
		}
		else 
		{
			btnLoad->visible = false;
			
			btnNewGame->visible = true;
			btnNewGame->secondary("", false);
			btnNewGame->setRect(buttonX, buttonY, Camera::mainCamera->width - buttonX * 2, BUTTON_HEIGHT);
		}
	
	}
	else 
	{
		unlock->visible = true;
		btnLoad->visible = false;
		btnNewGame->visible = false;	
	}
}

void StartScene::startNewGame()
{
	Dungeon::hero = NULL;
	InterlevelScene::mode = InterlevelScene::Mode::DESCEND;
	
	if (PixelDungeon::intro()) 
	//if (true)
	{
		PixelDungeon::intro(false);
		Game::switchScene(new IntroScene());
	}
	else 
	{
		Game::switchScene(new InterlevelScene());
	}
}

void StartScene::onBackPressed()
{
	PixelDungeon::switchNoFade(new TitleScene());
}