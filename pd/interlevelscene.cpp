#include "interlevelscene.h"
#include "game.h"
#include "wnderror.h"
#include "startscene.h"
#include "dungeon.h"
#include "gamescene.h"
#include "bpt.h"
#include "actor.h"
#include "wndstory.h"
#include "gamelog.h"
#include "statistics.h"
#include "hero.h"

const float InterlevelScene::TIME_TO_FADE = 0.3f;

const std::string InterlevelScene::TXT_DESCENDING = "lang.inter_desc";
const std::string InterlevelScene::TXT_ASCENDING = "lang.inter_asce";
const std::string InterlevelScene::TXT_LOADING = "lang.inter_load";
const std::string InterlevelScene::TXT_RESURRECTING = "lang.inter_resurr";
const std::string InterlevelScene::TXT_RETURNING = "lang.inter_return";
const std::string InterlevelScene::TXT_FALLING = "lang.inter_falling";

const std::string InterlevelScene::ERR_FILE_NOT_FOUND = "lang.inter_filenotfound";
const std::string InterlevelScene::ERR_GENERIC = "lang.inter_errgeneric";

InterlevelScene::Mode InterlevelScene::mode;

int InterlevelScene::returnDepth;
int InterlevelScene::returnPos;

bool InterlevelScene::noStory = false;

bool InterlevelScene::fallIntoPit;

namespace{
	void* InterlevelSceneThread(void* Param)
	{
		InterlevelScene* sce = (InterlevelScene*)Param;

		//try {
			//
			//Generator.reset();
			
		switch (sce->mode) 
		{
		case InterlevelScene::DESCEND:
			sce->descend();
			break;
		case InterlevelScene::ASCEND:
			sce->ascend();
			break;
		case InterlevelScene::CONTINUE:
			sce->restore();
			break;
		case InterlevelScene::RESURRECT:
			sce->resurrect();
			break;
		case InterlevelScene::RETURN:
			sce->returnTo();
			break;
		case InterlevelScene::FALL:
			sce->fall();
			break;
		}
			
		if ((Dungeon::depth % 5) == 0) 
		{
			//Sample.INSTANCE.load(Assets.SND_BOSS);
		}

		if (sce->phase == InterlevelScene::Phase::STATIC && sce->error.length() <= 0) 
		{
			sce->phase = InterlevelScene::Phase::FADE_OUT;
			sce->timeLeft = InterlevelScene::TIME_TO_FADE;
		}

		sce->threadEndFlag = true;
		return NULL;
	}
}
void InterlevelScene::init()
{
	PixelScene::init();

	std::string text;
	switch (mode) 
	{
	case DESCEND:
		text = TXT_DESCENDING;
		break;
	case ASCEND:
		text = TXT_ASCENDING;
		break;
	case CONTINUE:
		text = TXT_LOADING;
		break;
	case RESURRECT:
		text = TXT_RESURRECTING;
		break;
	case RETURN:
		text = TXT_RETURNING;
		break;
	case FALL:
		text = TXT_FALLING;
		break;
	}

	message = PixelScene::createText(BPT::getText(text), 9);
	message->measure();
	message->x = (Camera::mainCamera->width - message->getWidth()) / 2;
	message->y = (Camera::mainCamera->height - message->getHeight()) / 2;
	add(message);

	phase = Phase::FADE_IN;
	timeLeft = TIME_TO_FADE;

	threadEndFlag = false;
	pthread_t threadID;
	pthread_create(&threadID, NULL, InterlevelSceneThread, this);
}

namespace{
	class WndErrorNew :public WndError{
	public:
		WndErrorNew(const std::string& msg)
			:WndError(msg)
		{}

		virtual void onBackPressed()
		{			
			Game::switchScene(new StartScene());
			WndError::onBackPressed();
		}
	};
}

bool InterlevelScene::isThreadAlive()
{
	return !threadEndFlag;
	//int rc = pthread_kill(threadID, 0);
	//if (rc == ESRCH)
	//{
	//	return false;
	//}
	//else if (rc == EINVAL)
	//{
	//	return false;
	//}
	//else
	//{
	//	return true;
	//}
}

void InterlevelScene::update()
{
	PixelScene::update();

	float p = timeLeft / TIME_TO_FADE;

	switch (phase) {

	case FADE_IN:
		message->alpha(1 - p);
		if ((timeLeft -= Game::elapsed) <= 0) 
		{
			if (!isThreadAlive() && error.length() <= 0)
			{
				phase = Phase::FADE_OUT;
				timeLeft = TIME_TO_FADE;
			}
			else 
			{
				phase = Phase::STATIC;
			}
		}
		break;

	case FADE_OUT:
		message->alpha(p);
		if (mode == Mode::CONTINUE || (mode == Mode::DESCEND && Dungeon::depth == 1)) 
		{
			//Music.INSTANCE.volume(p);
		}
		if ((timeLeft -= Game::elapsed) <= 0) 
		{
			// TODO: 
			//Game::switchScene(new GameScene());
		}
		break;

	case STATIC:
		if (error.length() > 0) 
		{
			add(new WndErrorNew(error));
			error.clear();
		}
		break;
	}
}

void InterlevelScene::descend()
{
	/*Actor::fixTime();
	if (Dungeon::hero == NULL) 
	{
		Dungeon::init();
		if (noStory) 
		{
			Dungeon::chapters.insert(WndStory::ID_SEWERS);
			noStory = false;
		}
		GameLog::wipe();
	}
	else 
	{
		Dungeon::saveLevel();
	}
	
	Level* level = NULL;
	if (Dungeon::depth >= Statistics::deepestFloor) 
	{
		level = Dungeon::newLevel();
	}
	else 
	{
		Dungeon::depth++;
		level = Dungeon::loadLevel(Dungeon::hero->heroClass);
	}
	Dungeon::switchLevel(level, level->entrance);*/
}

void InterlevelScene::fall()
{
	Actor::fixTime();
	Dungeon::saveLevel();
	
	Level* level;
	if (Dungeon::depth >= Statistics::deepestFloor)
	{
		level = Dungeon::newLevel();
	}
	else 
	{
		Dungeon::depth++;
		level = Dungeon::loadLevel(Dungeon::hero->heroClass);
	}
	Dungeon::switchLevel(level, fallIntoPit ? level->pitCell() : level->randomRespawnCell());
}

void InterlevelScene::ascend()
{
	Actor::fixTime();
	
	Dungeon::saveLevel();
	Dungeon::depth--;
	Level* level = Dungeon::loadLevel(Dungeon::hero->heroClass);
	Dungeon::switchLevel(level, level->exit);
}

void InterlevelScene::returnTo()
{
	Actor::fixTime();
	
	Dungeon::saveLevel();
	Dungeon::depth = returnDepth;
	Level* level = Dungeon::loadLevel(Dungeon::hero->heroClass);
	Dungeon::switchLevel(level, Level::resizingNeeded ? level->adjustPos(returnPos) : returnPos);
}

void InterlevelScene::restore()
{
	Actor::fixTime();
	
	GameLog::wipe();
	
	Dungeon::loadGame(StartScene::curClass);
	if (Dungeon::depth == -1) 
	{
		Dungeon::depth = Statistics::deepestFloor;
		Dungeon::switchLevel(Dungeon::loadLevel(StartScene::curClass), -1);
	}
	else 
	{
		Level* level = Dungeon::loadLevel(StartScene::curClass);
		Dungeon::switchLevel(level, Level::resizingNeeded ? level->adjustPos(Dungeon::hero->pos) : Dungeon::hero->pos);
	}
}

void InterlevelScene::resurrect()
{
	Actor::fixTime();
	
	if (Dungeon::bossLevel()) 
	{
		Dungeon::hero->resurrect(Dungeon::depth);
		Dungeon::depth--;
		Level* level = Dungeon::newLevel();
		Dungeon::switchLevel(level, level->entrance);
	}
	else 
	{
		Dungeon::hero->resurrect(-1);
		Dungeon::resetLevel();
	}
}