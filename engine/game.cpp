#include "stdafx.h"
#include "game.h"
#include "timing.h"
#include "noosascript.h"
#include "touchscreen.h"
#include <SDL_ttf.h>

int Game::width = 640;
int Game::height = 960;

float Game::timeScale = 1.0f;
float Game::elapsed = 0.0f;

Game* Game::instance = NULL;

float Game::density = 1;

GameState Game::state = GameState::PLAY;

Game::Game()
{
	instance = this;

	_scene = NULL;
	_requestedScene = NULL;
	_requestedReset = true;

	_now = 0;
	_step = 0;
}

Game::~Game()
{

}

void Game::init()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	_window = SDL_CreateWindow("PixelDungeon", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	_context = SDL_GL_CreateContext(_window);

	SDL_GL_SetSwapInterval(1);
	glewInit();

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	std::printf("*** OpenGL Version: %s ***\n", glGetString(GL_VERSION));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_SCISSOR_TEST);

	TTF_Init();

	// setup sould
	int flag = MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG;
	if (Mix_Init(flag) & flag != flag)
	{
		std::string err = Mix_GetError();
		fatalError("Mix_Init error "+err);
	}

	// 44100 cd rate, more cpu power
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
	{
		std::string err = Mix_GetError();
		fatalError("Mix_OpenAudio error " + err);
	}
}

void Game::run()
{
	FPSLimiter fps;
	fps.setMaxFPS(60.0f);

	while (state == GameState::PLAY)
	{
		fps.begin();

		long rightNowTick = SDL_GetTicks();
		_step = (_now == 0 ? 0 : rightNowTick - _now);
		_now = rightNowTick;

		processInput();

		step();

		draw();

		SDL_GL_SwapWindow(_window);

		_fps = fps.end();
	}
}

void Game::processInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type)
		{
		case SDL_QUIT:
			state = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			_motionEvents.push_back(e);
			break;
		case SDL_KEYDOWN:
			
			break;
		case SDL_KEYUP:
			
			break;
		case SDL_MOUSEBUTTONDOWN:
			_motionEvents.push_back(e);
			break;
		case SDL_MOUSEBUTTONUP:
			_motionEvents.push_back(e);
			break;
		}
	}

	TouchScreen::processTouchEvents(_motionEvents);
	_motionEvents.clear();
}

void Game::switchScene(Scene* sce)
{
	instance->_requestedScene = sce;
	instance->_requestedReset = true;
}

void Game::vibrate(int milliseconds)
{
	// vibrate
}

void Game::switchScene()
{
	Camera::reset();

	if (_scene != NULL)
	{
		_scene->destroy();
		delete _scene;
	}

	_scene = _requestedScene;
	_scene->init();

	Game::elapsed = 0.0f;
	Game::timeScale = 1.0f;
}

void Game::step()
{
	if (_requestedReset)
	{
		_requestedReset = false;
		switchScene();
	}

	update();
}

void Game::update()
{
	Game::elapsed = Game::timeScale * _step * 0.001f;

	_scene->update();

	Camera::updateAll();
}

void Game::draw()
{
	NoosaScript::get()->resetCamera();
	glScissor(0, 0, width, height);
	//glClearDepth(1.0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	//glClearColor(0.0, 0.0, 0.0, 1.0);
	//glClearColor(1.0, 1.0, 1.0, 1.0);

	glActiveTexture(GL_TEXTURE0);

	_scene->draw();
}

void Game::finish()
{
	SDL_Quit();
	exit(0);
}