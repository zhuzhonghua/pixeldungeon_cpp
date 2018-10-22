#include "stdafx.h"

#include "music.h"

namespace Zengine{
Music* Music::_inst;

Music::Music()
:_music(NULL)
{

}
Music* Music::inst()
{
	if (_inst == NULL)
	{
		_inst = new Music();
	}

	return _inst;
}

void Music::play(const std::string & assetName, bool looping)
{
	if (_music)
	{
		Mix_FreeMusic(_music);
		_music = NULL;
	}

	// TODO: thread load
	_music = Mix_LoadMUS(assetName.c_str());
	Mix_PlayMusic(_music, -1);
}

void Music::pause()
{
	Mix_PauseMusic();
}

void Music::stop()
{
	if (_music)
	{
		Mix_FreeMusic(_music);
		_music = NULL;
	}
}

void Music::resume()
{
	Mix_ResumeMusic();
}

void Music::volume(int value)
{
	Mix_VolumeMusic(value);
}
};