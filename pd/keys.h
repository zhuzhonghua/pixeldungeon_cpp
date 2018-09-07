#pragma once

#include "SDL.h"
#include "signal.h"

#include <vector>

class Keys{
public:
	static const int BACK /*= KeyEvent.KEYCODE_BACK*/;
	static const int MENU /*= KeyEvent.KEYCODE_MENU*/;
	static const int VOLUME_UP /*= KeyEvent.KEYCODE_VOLUME_UP*/;
	static const int VOLUME_DOWN /*= KeyEvent.KEYCODE_VOLUME_DOWN*/;

	struct Key 
	{
		int code;
		bool pressed;

		Key(int code, bool pressed) 
		{
			this->code = code;
			this->pressed = pressed;
		}
	};

	static Signal<Key> event;

	static void processTouchEvents(std::vector<SDL_Event>& events);
};